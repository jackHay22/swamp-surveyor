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
  #define GRAVITY_PER_TICK 1

  /**
   * Constructor
   * @param x                  entity position x (center)
   * @param y                  entity position y (center)
   * @param w                  entity bounds width
   * @param h                  entity bounds height
   * @param anim_cfg_paths     the paths to the configuration files for each animation
   * @param renderer           the renderer for loading textures
   */
  entity_t::entity_t(int x, int y,
                     int w, int h,
                     const std::vector<std::string>& anim_cfg_paths,
                     SDL_Renderer& renderer,
                     bool debug)
    : x(x), y(y), w(w), h(h), last_x(x), last_y(y), anims(), debug(debug) {

      if (anim_cfg_paths.size() < 4) {
        throw exceptions::rsrc_exception_t("not enough entity animation paths provided");
      }

      //load animation frames (must be in order)
      for (const std::string& p : anim_cfg_paths) {
        //create the animation set and add to list
        anims.push_back(std::make_unique<anim_set_t>(p,renderer));
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
   */
  void entity_t::update() {
    //update the current animation
    anims.at(state)->update();
  }

  /**
   * Update this entity at the tick
   */
  void entity_t::update_x() {
    //store the previous position
    this->last_x = this->x;

    //check for walk
    if (state == MOVE_LEFT) {
      this->x -= 1;
    }

    if (state == MOVE_RIGHT) {
      this->x += 1;
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
   * Restore the previous position of the entity
   */
  void entity_t::step_back_x() {
    this->x = this->last_x;
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
    anims.at(state)->render(renderer,
                            (x - (w / 2)) - camera.x,
                            (y - (h / 2)) - camera.y);

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
