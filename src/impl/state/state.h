/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_STATE_H
#define _IO_JACKHAY_SWAMP_STATE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

namespace impl {
namespace state {

  /**
   * A Virtual state
   */
  struct state_t {
  protected:
    //the current camera
    SDL_Rect camera;

  public:
    //constructor
    state_t(SDL_Rect camera) : camera(camera) {}
    state_t(const state_t&) = delete;
    state_t& operator=(const state_t&) = delete;
    virtual ~state_t() {}

    /**
     * Handle some key event
     * @param e the event
     */
    virtual void handle_event(const SDL_Event& e) {}

    /**
     * Update this tile
     */
    virtual void update() {}

    /**
     * Render the current gamestate
     * @param renderer the renderer
     */
    virtual void render(SDL_Renderer& renderer) const {}
  };
}}

#endif /*_IO_JACKHAY_SWAMP_STATE_H*/
