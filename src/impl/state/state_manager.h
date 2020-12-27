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
#include "state.h"

namespace impl {
namespace state {

  /**
   * Manages thhe current game state
   */
  struct state_manager_t {
  private:
    //the game states
    std::vector<std::unique_ptr<state_t>> states;

    //the current state of the game
    enum {
      TITLE, LEVEL, PAUSE
    } current_state = TITLE;

  public:
    //constructor
    state_manager_t();
    state_manager_t(const state_manager_t&) = delete;
    state_manager_t& operator=(const state_manager_t&) = delete;

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
     * Update the current state
     */
    void update();

    /**
     * Render the current gamestate
     * @param renderer the renderer
     */
    void render(SDL_Renderer& renderer) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_STATE_MANAGER_H*/
