/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_STATE_H
#define _IO_JACKHAY_SWAMP_STATE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "state_manager.h"

namespace impl {
namespace state {

  class state_manager_t;

  /**
   * A Virtual state
   */
  struct state_t {
  private:
    //the current camera
    SDL_Rect _camera;
    //the debug camera
    SDL_Rect _debug_camera;
  protected:
    //the state state manager (used for state changes)
    state_manager_t& manager;
    //whether the debug camera is being used
    bool using_debug;

    /**
     * Get the debug camera
     * @return the debug camera
     */
    SDL_Rect& get_debug_camera();

    /**
     * Make a copy of the current camera
     * @return the camera
     */
    const SDL_Rect& get_active_camera() const;

    /**
     * Get the active camera
     * @return the camera
     */
    SDL_Rect& get_player_camera();

  public:
    //constructor
    state_t(state_manager_t& manager,
            SDL_Rect camera);
    state_t(const state_t&) = delete;
    state_t& operator=(const state_t&) = delete;
    virtual ~state_t() {}

    /**
     * Toggle the debug camera
     */
    void toggle_debug_camera();

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
     * @param debug    whether debug mode enabled
     */
    virtual void render(SDL_Renderer& renderer, bool debug) const {}

    /**
     * Render any debug info
     * @param renderer sdl renderer
     * @param font     loaded ttf font
     */
    virtual void render_debug_info(SDL_Renderer& renderer, TTF_Font& font) const {}
  };
}}

#endif /*_IO_JACKHAY_SWAMP_STATE_H*/
