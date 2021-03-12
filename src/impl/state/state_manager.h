/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_STATE_MANAGER_H
#define _IO_JACKHAY_SWAMP_STATE_MANAGER_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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

    //the pause state
    std::unique_ptr<state_t> pause_state;

    //configuration paths for lazy loading
    std::vector<std::string> deferred_cfgs;

    //the last deferred state that was loaded
    int last_loaded;

    //the lock for render/update on shared state
    std::shared_mutex lock;

    bool running;
    bool paused;

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

    //the window scale
    int window_scale;

  public:
    /**
     * Constructor
     * @param renderer the renderer for loading images
     * @param camera   the default camera
     * @param tile_dim the tile dimension
     * @param window_scale the scale of the window
     */
    state_manager_t(SDL_Renderer& renderer,
                    SDL_Rect& camera,
                    int tile_dim,
                    const std::string& font_path,
                    int window_scale,
                    bool debug);
    state_manager_t(const state_manager_t&) = delete;
    state_manager_t& operator=(const state_manager_t&) = delete;

    /**
     * Whether debug mode enabled
     * @return debug mode
     */
    bool is_debug() const { return debug; }

    /**
     * Reload the resources from configuration for the current map
     * Note: this should be called by the pause menu
     * (Note: this keeps the player in the same place)
     */
    void rsrc_reload();

    /**
     * Toggle the debug camera (if applicable)
     */
    void toggle_debug_camera();

    /**
     * Get the window scale
     * @return the scale of the window
     */
    int get_window_scale() const { return window_scale; }

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
     * @param idx_override whether to place the state in a specific position
     *        if not, -1
     */
    void add_state(std::unique_ptr<state_t> s,
                   int idx_override=-1);

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
     * Unpause the state (if paused)
     */
    void unpause();

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
     * @param debug    whether debug enabled
     */
    void render(SDL_Renderer& renderer, bool debug);

    /**
     * Render any debug info
     * @param renderer sdl renderer
     * @param font     loaded ttf font
     */
    void render_debug_info(SDL_Renderer& renderer, TTF_Font& font);
  };
}}

#endif /*_IO_JACKHAY_SWAMP_STATE_MANAGER_H*/
