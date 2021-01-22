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

  enum state_type {
    TITLE,
    SWAMP,
    TRACKS
  };

  /**
   * Manages thhe current game state
   */
  struct state_manager_t {
  private:
    //the game states
    std::vector<std::unique_ptr<state_t>> states;

    //configuration paths for lazy loading
    std::vector<std::string> deferred_cfgs;

    //the last deferred state that was loaded
    int last_loaded;

    //the lock for render/update on shared state
    std::shared_mutex lock;

    bool running;

    //the sdl renderer
    SDL_Renderer& renderer;

    //default camera
    SDL_Rect camera;

    //the tile dimension
    int tile_dim;

    //resource directory base path
    std::string base_path;
    //the font to use
    std::string font_path;

    //debug mode
    bool debug;

    //the current state
    state_type current_state;
    state_type last_state;

  public:
    /**
     * Constructor
     * @param renderer the renderer for loading images
     * @param camera   the default camera
     * @param tile_dim the tile dimension
     * @param debug    whether debug mode enabled
     */
    state_manager_t(SDL_Renderer& renderer,
                    SDL_Rect& camera,
                    int tile_dim,
                    bool debug);
    state_manager_t(const state_manager_t&) = delete;
    state_manager_t& operator=(const state_manager_t&) = delete;

    /**
     * Set the state to pause or unpause to previous
     * @param pause whether to pause (true) or unpause (false)
     */
    void set_pause(bool pause);

    /**
     * Set the current level state
     * @param name the state type
     */
    void set_state(state_type type);

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
     * Set the state paths in the manager but don't
     * load until the level is required
     * @param cfgs     the paths to the level configurations
     * @param base_path the resource directory base path
     * @param font_path the path to the font to use
     */
    void load_defer(const std::vector<std::string>& cfgs,
                    const std::string& base_path,
                    const std::string& font_path);

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
