/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_UI_COMPONENT_H
#define _IO_JACKHAY_UI_COMPONENT_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

namespace impl {
namespace ui {

  /**
   * A GUI component interface
   */
  struct component_t {

    /**
     * Whether a given x y position is in bounds
     * @param  x position x
     * @param  y position y
     * @return   whether this position is in bounds
     */
    virtual bool in_bounds(int x, int y) const = 0;

    /**
     * Called when a component is clicked
     */
    virtual void clicked() = 0;

    /**
     * Get the bounds of this component
     * @return the bounds of the component on the screen
     */
    virtual SDL_Rect bounds() const = 0;

    /**
     * Handle an SDL event (to calculate mouse position)
     */
    virtual void handle_event(const SDL_Event& e) = 0;

    /**
     * Render the var
     * @param renderer the renderer to use
     */
    virtual void render(SDL_Renderer& renderer) const = 0;
  };
}}

#endif /*_IO_JACKHAY_UI_COMPONENT_H*/
