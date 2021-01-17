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
  bool renderable_t::collides(const SDL_Rect& rect) const{
    //look for rectangular collision
    return ((bounds.x < (rect.x + rect.w)) &&
            ((bounds.x + bounds.w) > rect.x) &&
            (bounds.y < (rect.y + rect.h)) &&
            ((bounds.y + bounds.h) > rect.y));
  }
}}
