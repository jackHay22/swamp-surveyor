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
  protected:
    //the bounds of this renderable element
    SDL_Rect bounds;

    //whether the player can interact with this element
    bool interactive;

    //whether this env element is a solid object
    bool solid;

  public:
    //constructor takes bounds
    renderable_t(const SDL_Rect& bounds, bool interactive, bool solid)
      : bounds(bounds), interactive(interactive), solid(solid) {}
    renderable_t(const renderable_t&) = delete;
    renderable_t& operator=(const renderable_t&) = delete;

    virtual ~renderable_t() {}

    /**
     * Whether the element is *currently* interactive
     * @return whether this element can be interacted with by the player
     */
    virtual bool is_interactive() const { return interactive; }

    /**
     * Interact with this element
     * @param x the player position x
     * @param y the player position y
     */
    virtual void interact(int x, int y) {}

    /**
     * Return whether this is a solid object
     * @return whether this element is solid
     */
    virtual bool is_solid() const { return solid; }

    /**
     * Get the damage value of this renderable element
     * @return the damage this environment element does
     */
    virtual int get_damage() const { return 0; }

    /**
     * Get the bounds of this environmental object
     * @return a const ref to the object bounds
     */
    const SDL_Rect& get_bounds() const { return bounds; }

    /**
     * Check if this element is in view of the camera
     * @param  recr the collision box
     * @return
     */
    virtual bool is_collided(const SDL_Rect& rect) const;

    /**
     * Check if some coordinate collides with this element
     * @param  x the x coordinate
     * @param  y the y coordinate
     * @return   whether this position collides with the bounds
     */
    virtual bool is_collided(int x, int y) const;

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
