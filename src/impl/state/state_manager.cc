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
    std::unique_lock<std::shared_mutex> state_lock(lock,std::defer_lock);

    //if lock acquired, render
    if (state_lock.try_lock()) {
      states.at(current_state)->render(renderer);
    }
  }
}}
