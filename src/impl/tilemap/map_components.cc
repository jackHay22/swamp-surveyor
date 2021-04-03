/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "map_components.h"
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
  map_components_t::map_components_t()
    : anims(),
      anim_bounds(),
      statics(),
      static_bounds() {}

  /**
   * Check for a collision between two rectangles
   * @param camera camera
   * @param other  the bounds of some component
   */
  bool map_components_t::camera_collides(const SDL_Rect& camera,
                                         const SDL_Rect& other) const {
    return ((camera.x < (other.x + other.w)) &&
            ((camera.x + camera.w) > other.x) &&
            (camera.y < (other.y + other.h)) &&
            ((camera.y + camera.h) > other.y));
  }

  /**
   * Free static textures
   */
  map_components_t::~map_components_t() {
    for (size_t i=0; i<statics.size(); i++) {
      if (statics.at(i) != NULL) {
        SDL_DestroyTexture(statics.at(i));
      }
    }
  }

  /**
   * Add a static component
   * (Assumes ownership of texture)
   * @param texture the texture
   * @param bounds  the bounds
   */
  void map_components_t::add_static(SDL_Texture *texture, SDL_Rect&& bounds) {
    statics.push_back(texture);
    static_bounds.push_back(bounds);
  }

  /**
   * Add an animated component
   * @param texture  the texture
   * @param bounds   the bounds
   * @param frames   the number of frames
   * @param duration the duration of each frame
   */
  void map_components_t::add_dynamic(SDL_Texture *texture,
                                     SDL_Rect&& bounds,
                                     int frames,
                                     int duration) {
    anims.push_back(std::make_unique<entity::anim_set_t>(
      texture,
      bounds.w,
      bounds.h,
      frames,
      duration
    ));

    //actual width corrected for frame count
    int actual_width = bounds.w / frames;

    anim_bounds.push_back({
      bounds.x,
      bounds.y,
      actual_width,
      bounds.h
    });
  }

  /**
   * Update any animated textures
   */
  void map_components_t::update() {
    for (size_t i=0; i<anims.size(); i++) {
      anims.at(i)->update();
    }
  }

  /**
   * Render the texture
   * @param renderer sdl renderer
   * @param camera   camera position
   */
  void map_components_t::render(SDL_Renderer& renderer, const SDL_Rect& camera) const {

    //draw statics first
    for (size_t i=0; i<statics.size(); i++) {

      //check for a collision
      if (camera_collides(camera,static_bounds.at(i))) {
        const SDL_Rect& curr_bounds = static_bounds.at(i);

        SDL_Rect sample_bounds = {0,
                                  0,
                                  curr_bounds.w,
                                  curr_bounds.h};
        SDL_Rect image_bounds = {curr_bounds.x - camera.x,
                                 curr_bounds.y - camera.y,
                                 curr_bounds.w,
                                 curr_bounds.h};

        SDL_RenderCopy(&renderer,
                       statics.at(i),
                       &sample_bounds,
                       &image_bounds);
      }
    }

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
