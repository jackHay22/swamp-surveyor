/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "engine.h"

namespace impl {
namespace engine {

  /**
   * Start the update thread
   * @param manager the gamestate manager
   * @return whether the thread was started successfully
   */
  bool start_update_thread(std::shared_ptr<state::state_manager_t> manager) {
    return true;
  }

  /**
   * Start the render loop
   * @param renderer the sdl renderer
   * @manager the gamestate manager
   * @return success or failure
   */
  bool start_renderer(SDL_Renderer& renderer,
                      std::shared_ptr<state::state_manager_t> manager) {
    return true;
  }
}}
