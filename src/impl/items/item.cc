/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "item.h"
#include "../utils.h"

namespace impl {
namespace items {

  //the number of ticks for animation update
  #define TICKS_PER_UPDATE 6
  #define GRAVITY_PER_TICK 2
  #define PICK_UP_RADIUS 6
  #define PICKUP_TICKS 3

  /**
   * Constructor
   * @param x            position of the item x
   * @param y            position of the item y
   * @param texture_path the path to the texture
   * @param renderer     the renderer for loading the texture
   * @param debug        whether debug mode enabled
   */
  item_t::item_t(int x, int y,
                 const std::string& texture_path,
                 SDL_Renderer& renderer,
                 bool debug)
    : x(x), y(y),
      position_ticks(0),
      position_up(false),
      picked_up(false),
      target_x(x),
      target_y(y),
      debug(debug) {

    //load the texture
    texture = utils::load_texture(texture_path,
                                  renderer,
                                  texture_w,
                                  texture_h);
  }

  /**
   * Free texture
   */
  item_t::~item_t() {
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }

  /**
   * Player picks up this item, animate movement
   * @param x the player position x
   * @param y the player position y
   */
  void item_t::pick_up(int x, int y) {
    if (!picked_up) {
      picked_up = true;
      position_ticks = 0;
      //set the pick up target
      target_x = x;
      target_y = y;
    }
  }

  /**
   * Check if a bounding box collides with this item
   * @param  bounds the bounds to check
   * @return        whether the bounds collide
   */
  bool item_t::is_collided(const SDL_Rect& bounds) const {
    SDL_Rect item_bounds = this->get_bounds();
    //set the pick up radius
    item_bounds.x -= PICK_UP_RADIUS;
    item_bounds.y -= PICK_UP_RADIUS;
    item_bounds.w += (2 * PICK_UP_RADIUS);
    item_bounds.h += (2 * PICK_UP_RADIUS);
    //look for rectangular collision
    return ((item_bounds.x < (bounds.x + bounds.w)) &&
            ((item_bounds.x + item_bounds.w) > bounds.x) &&
            (item_bounds.y < (bounds.y + bounds.h)) &&
            ((item_bounds.y + item_bounds.h) > bounds.y));
  }

  /**
   * Get the bounding box for this item
   * @return the bounding box for this item
   */
  SDL_Rect item_t::get_bounds() const {
    SDL_Rect bounds = {x - (texture_w / 2),
                       y - (texture_h / 2),
                       texture_w,
                       texture_h};
    return bounds;
  }

  /**
   * Update the item
   * @param map the tilemap
   */
  void item_t::update(const tilemap::tilemap_t& map) {
    //check if the item is resting or being picked up
    if (!picked_up) {
      //check if need to update bounce
      if (position_ticks >= TICKS_PER_UPDATE) {
        position_ticks = 0;
        position_up = !position_up;

      } else {
        position_ticks++;
      }

      //apply gravity and check for tilemap intersection
      int last_y = y;
      y += GRAVITY_PER_TICK;

      //if there is a collision, revert y position
      if (map.is_collided(get_bounds())) {
        y = last_y;
      }
    } else if (position_ticks < PICKUP_TICKS) {
      int dist_x = (target_x - x);
      int dist_y = (target_y - y);

      //based on direction move the item
      if (dist_x - 1 > 0) {
        this->x -= (dist_x / 2);
      } else if (dist_x + 1 < 0) {
        this->x += (dist_x / 2);
      }
      if (dist_y - 1 > 0) {
        this->y -= (dist_y / 2);
      } else if (dist_y + 1 < 0) {
        this->y += (dist_y / 2);
      }

      position_ticks++;
    }
    //otherwise, fully picked up
  }

  /**
   * Render the item
   * @param renderer the renderer to use
   * @param camera   the camera
   */
  void item_t::render(SDL_Renderer& renderer, const SDL_Rect& camera) const {
    if (!picked_up || (position_ticks < PICKUP_TICKS)) {
      //create a clip for the current frame
      SDL_Rect sample_bounds = {0,0,texture_w,texture_h};

      //the x y position to render at
      SDL_Rect image_bounds = {(x - (texture_w / 2)) - camera.x,
                               (y - (texture_h / 2) - position_up) - camera.y,
                               texture_w,texture_h};

      //render the texture
      SDL_RenderCopy(&renderer,
                     texture,
                     &sample_bounds,
                     &image_bounds);

      if (debug) {
        SDL_Rect pickup_bounds = {(x - (texture_w / 2) - PICK_UP_RADIUS) - camera.x,
                                  (y - (texture_h / 2) - position_up - PICK_UP_RADIUS) - camera.y,
                                  texture_w + (2 * PICK_UP_RADIUS),
                                  texture_h + (2 * PICK_UP_RADIUS)};
        //set the draw color
        SDL_SetRenderDrawColor(&renderer,74,7,100,255);
        //render the bounds
        SDL_RenderDrawRect(&renderer,&pickup_bounds);
      }

    }
  }
}}
