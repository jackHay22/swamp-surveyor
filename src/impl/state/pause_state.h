/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_PAUSE_STATE_H
#define _IO_JACKHAY_SWAMP_PAUSE_STATE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "state.h"
#include "state_manager.h"
#include <string>
#include <memory>
#include "../ui/window.h"

namespace impl {
namespace state {

  /**
   * The starting state displaying options
   */
  struct pause_state_t : public state_t {
  private:
    //the pause window
    std::unique_ptr<ui::window_t> window;

  public:
    /**
     * Constructor
     * @param width    the window width
     * @param height   the window height
     * @param manager  the state manager
     * @param renderer the renderer for loading textures
     */
    pause_state_t(int width,
                  int height,
                  int window_scale,
                  state_manager_t& manager,
                  const std::string& font_path,
                  SDL_Renderer& renderer);
    pause_state_t(const pause_state_t&) = delete;
    pause_state_t& operator=(const pause_state_t&) = delete;

    /**
     * Handle some key event
     * @param e the event
     */
    void handle_event(const SDL_Event& e);

    /**
     * Render the current gamestate
     * @param renderer the renderer
     * @param debug    whether debug mode enabled
     */
    void render(SDL_Renderer& renderer, bool debug) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_PAUSE_STATE_H*/
