/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "renderable.h"

namespace impl {
namespace environment {

  /**
   * Check if this element is in view of the camera
   * @param  recr the collision box
   * @return
   */
  bool renderable_t::is_collided(const SDL_Rect& rect) const{
    //look for rectangular collision
    return ((bounds.x < (rect.x + rect.w)) &&
            ((bounds.x + bounds.w) > rect.x) &&
            (bounds.y < (rect.y + rect.h)) &&
            ((bounds.y + bounds.h) > rect.y));
  }

  /**
   * Check if some coordinate collides with this element
   * @param  x the x coordinate
   * @param  y the y coordinate
   * @return   whether this position collides with the bounds
   */
  bool renderable_t::is_collided(int x, int y) const {
    return (x >= bounds.x) &&
           (x < (bounds.x + bounds.w)) &&
           (y >= bounds.y) &&
           (y < (bounds.y + bounds.h));
  }
}}
