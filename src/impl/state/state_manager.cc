/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "state_manager.h"

namespace impl {
namespace state {

  /**
   * Constructor
   */
  state_manager_t::state_manager_t() : states() {}

  /**
   * Handle some keypress event
   * @param e the keypress event
   */
  void state_manager_t::handle_event(const SDL_Event& e) {
    states.at(current_state)->handle_event(e);
  }

  /**
   * Add a state to the manager
   * @param s the state
   */
  void state_manager_t::add_state(std::unique_ptr<state_t> s) {
    states.push_back(std::move(s));
  }

  /**
   * Update this tile
   */
  void state_manager_t::update() {
    states.at(current_state)->update();
  }

  /**
   * Render the current gamestate
   * @param renderer the renderer
   */
  void state_manager_t::render(SDL_Renderer& renderer) const {
    states.at(current_state)->render(renderer);
  }
}}
