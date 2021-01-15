/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENGINE_H
#define _IO_JACKHAY_SWAMP_ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include "state/state_manager.h"

namespace impl {
namespace engine {

  /**
   * Start the update thread
   * @param manager the gamestate manager
   * @return whether the thread was started successfully
   */
  bool start_update_thread(std::shared_ptr<state::state_manager_t> manager);

  /**
   * Start the render loop
   * @param renderer the sdl renderer
   * @param manager the gamestate manager
   * @param debug whether debug enabled
   * @param debug_font_name the font to use
   * @return success or failure
   */
  bool start_renderer(SDL_Renderer& renderer,
                      std::shared_ptr<state::state_manager_t> manager,
                      const bool debug,
                      const std::string& debug_font_name);
}}

#endif /*_IO_JACKHAY_SWAMP_ENGINE_H*/
