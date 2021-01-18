/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "entity.h"
#include "../exceptions.h"
#include <iostream>

namespace impl {
namespace entity {

  //the gravity applied to an entity per tick
  #define GRAVITY_PER_TICK 2
  #define CLIMB_FRAMES 6
  #define WATER_FRAMES 2
  #define STARTING_HEALTH 1000
  #define DAMAGE_TICKS 4

  /**
   * Constructor
   * @param x                  entity position x (center)
   * @param y                  entity position y (center)
   * @param w                  entity bounds width
   * @param h                  entity bounds height
   * @param anim_cfg_paths     the paths to the configuration files for each animation
   * @param renderer           the renderer for loading textures
   * @param tile_dim           the dimensions of tiles
   */
  entity_t::entity_t(int x, int y,
                     int w, int h,
                     const std::vector<std::string>& anim_cfg_paths,
                     SDL_Renderer& renderer,
                     int tile_dim,
                     bool debug)
    : x(x), y(y),
      w(w), h(h),
      health(STARTING_HEALTH),
      damaged_ticks(0),
      last_x(x),
      last_y(y),
      tile_dim(tile_dim),
      facing_left(false),
      climb_counter(CLIMB_FRAMES),
      water_counter(WATER_FRAMES),
      anims(), debug(debug) {

    if (anim_cfg_paths.size() < 8) {
      throw exceptions::rsrc_exception_t("not enough entity animation paths provided");
    }

    //load animation frames (must be in order)
    for (const std::string& p : anim_cfg_paths) {
      //create the animation set and add to list
      anims.push_back(std::make_unique<anim_set_t>(p,renderer));
    }
  }

  /**
   * Do damage to this entity by some magnitude
   * @param magnitude the amount off damage to do
   */
  void entity_t::do_damage(int magnitude) {
    if (magnitude > 0) {
      this->health -= magnitude;
      this->damaged_ticks = DAMAGE_TICKS;
    }
  }

  /**
   * Get the bounding box for this entity
   * @return the bounding box for this entity
   */
  SDL_Rect entity_t::get_bounds() const {
    //generate bounding box
    SDL_Rect r = {x - (w / 2),
                  y - (h / 2),
                  w,h};
    return r;
  }

  /**
   * Get the center of the entity
   * @param x the x position set by the call
   * @param y the y position set by the call
   */
  void entity_t::get_center(int& x, int& y) const {
    x = this->x;
    y = this->y;
  }

  /**
   * Update the entity (after directional updates)
   * @param map the tilemap
   * @param env_elements environmental elements that can be interacted
   */
  void entity_t::update(const tilemap::tilemap_t& /*map*/,
                        std::vector<std::shared_ptr<environment::renderable_t>>& /*env_elements*/) {
    //update the current animation
    anims.at(state * 2 + !facing_left)->update();

    //check if damaged
    if (damaged_ticks > 0) {
      damaged_ticks--;
    }
  }

  /**
   * Update this entity at the tick
   */
  void entity_t::update_x(const tilemap::tilemap_t& map) {
    //store the previous position
    this->last_x = this->x;

    //get the current player bounds
    SDL_Rect current_bounds = this->get_bounds();

    //check if the player is in liquid
    bool in_liquid = map.is_liquid(current_bounds.x + (current_bounds.w / 2),
                                   current_bounds.y + current_bounds.h - 4);

    //if in liquid set the animation slow
    anims.at(state)->set_slow(in_liquid);

    //check for walk
    if (state == MOVE) {
      //check if the player is in liquid
      if (in_liquid) {
        if (water_counter <= 0) {
          this->x += 1 - (2 * facing_left);
          water_counter = WATER_FRAMES;
        } else {
          water_counter--;
        }
      } else {
        this->x += 1 + (-2 * facing_left);
      }

    } else if (state == CLIMB) {
      if (climb_counter == 0) {
        //move the player up the surface
        this->x += tile_dim - (2 * tile_dim * facing_left);
        this->y -= tile_dim;
        state = MOVE;

      } else {
        climb_counter--;
      }
    }
  }

  /**
   * Update this entity at the tick in the y direction
   */
  void entity_t::update_y() {
    //store the previous position
    this->last_y = this->y;

    //update the y positon of the entity
    this->y += GRAVITY_PER_TICK;
  }

  /**
   * Called when entity collides in the x direction
   * @param layer used to determine if the entity can step up
   */
  void entity_t::step_back_x(const tilemap::tilemap_t& map) {
    //get the current player position
    SDL_Rect current_bounds = this->get_bounds();

    //stop the player (boundary can't be climbed)
    this->x = this->last_x;

    //make sure the entity is not already climbing
    if (state != CLIMB) {
      //the lowest collision check height
      int base_height = current_bounds.y + current_bounds.h - 2;

      //check for a 1-high collision
      if ((state == MOVE) && !facing_left) {
        //check upper and lower adjacent tiles
        if (map.is_solid(current_bounds.x + current_bounds.w + 2,
                         base_height) &&
            !map.is_solid(current_bounds.x + current_bounds.w + 2,
                          base_height - tile_dim)) {
          //climb mode
          state = CLIMB;
          climb_counter = CLIMB_FRAMES;
          return;
        }

      } else if (state == MOVE) {
        //check upper and lower adjacent tiles
        if (map.is_solid(current_bounds.x - 2,
                         base_height) &&
            !map.is_solid(current_bounds.x - 2,
                          base_height - tile_dim)) {
          //climb mode
          state = CLIMB;
          climb_counter = CLIMB_FRAMES;
          return;
        }
      }
    }
  }

  /**
   * Restore the previous position of the entity
   */
  void entity_t::step_back_y() {
    this->y = this->last_y;
  }

  /**
   * Render the entity
   * @param renderer the renderer to use
   * @param camera   the camera
   */
  void entity_t::render(SDL_Renderer& renderer, const SDL_Rect& camera) const {
    //render the current animation
    anims.at(state * 2 + !facing_left)->render(renderer,
                                               x - camera.x,
                                               y - camera.y);

    //render the damage indicator
    if (damaged_ticks > 0) {
      SDL_Rect bounds = {0,0,camera.w,camera.h};
      //render damage (TEMP)
      SDL_SetRenderDrawColor(&renderer,255,0,0,255);
      SDL_RenderDrawRect(&renderer,&bounds);
    }

    if (debug) {
      //get the current bounds (corrected by camera view)
      SDL_Rect bounds = {(x - (w / 2)) - camera.x,
                         (y - (h / 2)) - camera.y,
                         w,h};

      //set the draw color
      SDL_SetRenderDrawColor(&renderer,0,255,0,127);

      //render the bounds
      SDL_RenderDrawRect(&renderer,&bounds);
    }
  }
}}
