/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TILEMAP_MAP_COMPONENTS_H
#define _IO_JACKHAY_SWAMP_TILEMAP_MAP_COMPONENTS_H

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
  struct map_components_t {
  private:
    //any animated components
    std::vector<std::unique_ptr<entity::anim_set_t>> anims;
    //bounds of animated components
    std::vector<SDL_Rect> anim_bounds;
    //static components
    std::vector<SDL_Texture*> statics;
    //bounds of static components
    std::vector<SDL_Rect> static_bounds;

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
    map_components_t();
    map_components_t(const map_components_t&) = delete;
    map_components_t& operator=(const map_components_t&) = delete;

    /**
     * Free static textures
     */
    ~map_components_t();

    /**
     * Add a static component
     * (Assumes ownership of texture)
     * @param texture the texture
     * @param bounds  the bounds
     */
    void add_static(SDL_Texture *texture, SDL_Rect&& bounds);

    /**
     * Add an animated component
     * @param texture  the texture
     * @param bounds   the bounds
     * @param frames   the number of frames
     * @param duration the duration of each frame
     */
    void add_dynamic(SDL_Texture *texture,
                     SDL_Rect&& bounds,
                     int frames,
                     int duration);

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

#endif /*_IO_JACKHAY_SWAMP_TILEMAP_MAP_COMPONENTS_H*/
