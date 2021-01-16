/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_RENDERABLE_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_RENDERABLE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "../entity/player.h"

namespace impl {
namespace environment {

  /**
   * Some renderable component in the environment
   */
  struct renderable_t {
  protected:
    //the bounds of this renderable element
    SDL_Rect bounds;

  public:
    //constructor takes bounds
    renderable_t(const SDL_Rect& bounds)
      : bounds(bounds) {}
    renderable_t(const renderable_t&) = delete;
    renderable_t& operator=(const renderable_t&) = delete;

    virtual ~renderable_t() {}

    /**
     * Check if this element is in view of the camera
     * @param  recr the collision box
     * @return
     */
    bool collides(const SDL_Rect& rect) const;

    /**
     * If the player has collided with the bounding box, do
     * any environment interaction
     * @param player the player
     */
    virtual void player_interact(entity::player_t& player) {}

    /**
     * Update the renderable component
     */
    virtual void update() {}

    /**
     * Render the component
     * @param renderer the renderer to use
     * @param camera   the camera
     */
    virtual void render(SDL_Renderer& renderer, const SDL_Rect& camera) const {}
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_RENDERABLE_H*/
