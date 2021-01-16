/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_STATE_MANAGER_H
#define _IO_JACKHAY_SWAMP_STATE_MANAGER_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <shared_mutex>
#include "state.h"

namespace impl {
namespace state {

  class state_t;

  /**
   * Manages thhe current game state
   */
  struct state_manager_t {
  private:
    //the game states
    std::vector<std::unique_ptr<state_t>> states;

    //the lock for render/update on shared state
    std::shared_mutex lock;

    bool running = true;

    //the current state of the game
    //and the previous state for storage when paused
    enum {
      TITLE, LEVEL1, PAUSE
    } current_state = TITLE, last_state = TITLE;

  public:
    //constructor
    state_manager_t();
    state_manager_t(const state_manager_t&) = delete;
    state_manager_t& operator=(const state_manager_t&) = delete;

    /**
     * Set the state to pause or unpause to previous
     * @param pause whether to pause (true) or unpause (false)
     */
    void set_pause(bool pause);

    /**
     * Get the next level state
     */
    void next_level_state();

    /**
     * Handle some keypress event
     * @param e the keypress event
     */
    void handle_event(const SDL_Event& e);

    /**
     * Add a state to the manager
     * @param s the state
     */
    void add_state(std::unique_ptr<state_t> s);

    /**
     * Check whether the game is running
     * Note: not const; needs to lock mutex
     * @return whether the game is running
     */
    bool is_running();

    /**
     * Set the running state
     * @param running the state to set
     */
    void set_running(bool running);

    /**
     * Update the current state
     */
    void update();

    /**
     * Render the current gamestate
     * @param renderer the renderer
     */
    void render(SDL_Renderer& renderer);
  };
}}

#endif /*_IO_JACKHAY_SWAMP_STATE_MANAGER_H*/
