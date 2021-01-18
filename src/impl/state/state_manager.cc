/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "state_manager.h"
#include <iostream>

namespace impl {
namespace state {

  /**
   * Constructor
   */
  state_manager_t::state_manager_t() : states() {}

  /**
   * Set the state to pause or unpause to previous
   * @param pause whether to pause (true) or unpause (false)
   */
  void state_manager_t::set_pause(bool pause) {
    if (pause) {
      last_state = current_state;
      current_state = PAUSE;
    } else if (current_state == PAUSE) {
      current_state = last_state;
    }
  }

  /**
   * Get the next level state
   */
  void state_manager_t::next_level_state() {
    if (current_state == TITLE) {
      current_state = LEVEL1;
    }
  }

  /**
   * Handle some keypress event
   * @param e the keypress event
   */
  void state_manager_t::handle_event(const SDL_Event& e) {
    std::unique_lock<std::shared_mutex> state_lock(lock);
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
   * Check whether the game is running
   * @return whether the game is running
   */
  bool state_manager_t::is_running() {
    std::shared_lock<std::shared_mutex> state_lock(lock);
    return this->running;
  }

  /**
   * Set the running state
   * @param running the state to set
   */
  void state_manager_t::set_running(bool running) {
    std::unique_lock<std::shared_mutex> state_lock(lock);
    this->running = running;
  }

  /**
   * Update this tile
   */
  void state_manager_t::update() {
    //get a blocking lock on the state
    std::unique_lock<std::shared_mutex> state_lock(lock);

    //update the state
    states.at(current_state)->update();
  }

  /**
   * Render the current gamestate
   * @param renderer the renderer
   */
  void state_manager_t::render(SDL_Renderer& renderer) {
    //attempt to lock the state
    std::unique_lock<std::shared_mutex> state_lock(lock);

    //render the state
    states.at(current_state)->render(renderer);
  }
}}
