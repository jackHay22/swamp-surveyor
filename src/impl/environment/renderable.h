/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_RENDERABLE_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_RENDERABLE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

namespace impl {
namespace environment {

  /**
   * Some renderable component in the environment
   */
  struct renderable_t {
  public:
    //constructor
    renderable_t();
    renderable_t(const renderable_t&) = delete;
    renderable_t& operator=(const renderable_t&) = delete;

    virtual ~entity_t() {}

    /**
     * Update the renderable component
     */
    virtual void update();

    /**
     * Render the component
     * @param renderer the renderer to use
     * @param camera   the camera
     */
    virtual void render(SDL_Renderer& renderer, const SDL_Rect& camera) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_RENDERABLE_H*/
