/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "state_manager.h"
#include "state_builder.h"
#include "../logger.h"

namespace impl {
namespace state {

  /**
   * Constructor
   * @param renderer the renderer for loading images
   * @param camera   the default camera
   * @param tile_dim the tile dimension
   * @param debug    whether debug mode enabled
   */
  state_manager_t::state_manager_t(SDL_Renderer& renderer,
                                   SDL_Rect& camera,
                                   int tile_dim,
                                   bool debug)
    : states(),
      deferred_cfgs(),
      last_loaded(-1),
      running(true),
      renderer(renderer),
      camera(camera),
      tile_dim(tile_dim),
      debug(debug),
      current_state(TITLE),
      last_state(TITLE) {}

  /**
   * Set the state to pause or unpause to previous
   * @param pause whether to pause (true) or unpause (false)
   */
  void state_manager_t::set_pause(bool pause) {
    // if (pause) {
    //   last_state = current_state;
    //   current_state = PAUSE;
    // } else if (current_state == PAUSE) {
    //   current_state = last_state;
    // }
  }

  /**
   * Set the current level state
   * @param name the state type
   */
  void state_manager_t::set_state(state_type type) {
    //check if the state is not yet loaded
    if (type >= states.size()) {
      //lazily load the next state
      last_loaded++;

      //attempt to load from deferred cfg list
      if (debug) {
        logger::log_info("loading state " +
                         deferred_cfgs.at(last_loaded));
      }

      if (last_loaded < deferred_cfgs.size()) {
        //load a new state (state_builder.h)
        load_tm_state(*this,
                      deferred_cfgs.at(last_loaded),
                      renderer,
                      camera,
                      tile_dim,
                      debug);
      } else {
        logger::log_err("no cfg provided for next level");
        return;
      }
    }

    //set the current state
    current_state = type;
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
   * Set the state paths in the manager but don't
   * load until the level is required
   * @param cfgs     the paths to the level configurations
   */
  void state_manager_t::load_defer(const std::vector<std::string>& cfgs) {
    //set the deferred states
    deferred_cfgs = cfgs;
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
