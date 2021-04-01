/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "near_ground.h"
#include "../environment/texture_constructor.h"
#include "../environment/proc_generation.h"
#include "noise.h"

namespace impl {
namespace tilemap {

  #define MED_LIGHT_GREEN_R 104
  #define MED_LIGHT_GREEN_G 156
  #define MED_LIGHT_GREEN_B 91

  #define GROUND_RANGE 16

  /**
   * Constructor
   */
  near_ground_t::near_ground_t()
    : anims(),
      anim_bounds() {}

  /**
   * Check for a collision between two rectangles
   * @param camera camera
   * @param other  the bounds of some component
   */
  bool near_ground_t::camera_collides(const SDL_Rect& camera,
                                      const SDL_Rect& other) const {
    return ((camera.x < (other.x + other.w)) &&
            ((camera.x + camera.w) > other.x) &&
            (camera.y < (other.y + other.h)) &&
            ((camera.y + camera.h) > other.y));
  }

  /**
   * Update any animated textures
   */
  void near_ground_t::update() {
    for (size_t i=0; i<anims.size(); i++) {
      anims.at(i)->update();
    }
  }

  /**
   * Render the texture
   * @param renderer sdl renderer
   * @param camera   camera position
   */
  void near_ground_t::render(SDL_Renderer& renderer, const SDL_Rect& camera) const {

    //draw animations
    for (size_t i=0; i<anims.size(); i++) {
      //check that anim in camera
      if (camera_collides(camera,anim_bounds.at(i))) {
        const SDL_Rect& curr_bounds = anim_bounds.at(i);
        //render the animation
        anims.at(i)->render(renderer,
                            curr_bounds.x - (curr_bounds.w / 2) - camera.x,
                            curr_bounds.y - (curr_bounds.h / 2) - camera.y,
                            false);
      }
    }
  }

}}
