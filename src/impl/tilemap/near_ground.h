/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TILEMAP_NEAR_GROUND_H
#define _IO_JACKHAY_SWAMP_TILEMAP_NEAR_GROUND_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <vector>
#include <utility>
#include <memory>
#include "../entity/anim_set.h"

namespace impl {
namespace tilemap {

  /*
   * Procedurally generated background
   * Contains foliage, etc
   */
  struct near_ground_t {
  private:
    //any animated components
    std::vector<std::unique_ptr<entity::anim_set_t>> anims;
    //bounds of animated components
    std::vector<SDL_Rect> anim_bounds;

    /**
     * Check for a collision between two rectangles
     * @param camera camera
     * @param other  the bounds of some component
     */
    bool camera_collides(const SDL_Rect& camera, const SDL_Rect& other) const;

  public:
    /**
     * Constructor
     */
    near_ground_t();
    near_ground_t(const near_ground_t&) = delete;
    near_ground_t& operator=(const near_ground_t&) = delete;

    /**
     * Update any animated textures
     */
    void update();

    /**
     * Render the texture
     * @param renderer sdl renderer
     * @param camera   camera position
     */
    void render(SDL_Renderer& renderer, const SDL_Rect& camera) const;
  };

}}

#endif /*_IO_JACKHAY_SWAMP_TILEMAP_NEAR_GROUND_H*/
