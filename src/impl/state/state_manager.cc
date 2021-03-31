/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "state_manager.h"
#include "state_builder.h"
#include "../logger.h"
#include "tilemap_state.h"
#include "pause_state.h"
#include "../tilemap/procedural_tilemap.h"

namespace impl {
namespace state {

  /**
   * Constructor
   * @param renderer the renderer for loading images
   * @param camera   the default camera
   * @param tile_dim the tile dimension
   * @param debug mode
   */
  state_manager_t::state_manager_t(SDL_Renderer& renderer,
                                   SDL_Rect& camera,
                                   int tile_dim,
                                   const std::string& font_path,
                                   int window_scale,
                                   bool debug)
    : states(),
      pause_state(std::make_unique<pause_state_t>(camera.w,
                                                  camera.h,
                                                  window_scale,
                                                  *this,
                                                  font_path,
                                                  renderer)),
      deferred_cfgs(),
      last_loaded(-1),
      running(true),
      paused(false),
      renderer(renderer),
      camera(camera),
      tile_dim(tile_dim),
      base_path("resources/"),
      font_path(font_path),
      debug(debug),
      current_state(TITLE_STATE),
      last_state(TITLE_STATE),
      window_scale(window_scale) {}

  /**
   * Reload the resources from configuration for the current map
   * Note: this should be called by the pause menu
   * (Note: this keeps the player in the same place)
   */
  void state_manager_t::rsrc_reload() {
    //keep the player
    if (tilemap_state_t *curr_tilemap = dynamic_cast<tilemap_state_t*>(states.at(current_state).get())) {

      if (curr_tilemap->is_procedural()) {
        //regen
        this->new_swamp();
      } else {
        //get the current player
        std::shared_ptr<entity::player_t> player = curr_tilemap->get_player();

        bool using_debug = false;
        SDL_Rect old_camera = curr_tilemap->get_camera(using_debug);

        //get the current camera (applicable if the view is locked)

        if (debug) {
          logger::log_info("loading state " +
                           deferred_cfgs.at(last_state));
        }

        //reload the state
        load_tm_state(*this,
                      curr_tilemap->get_cfg(),
                      renderer,
                      camera,
                      tile_dim,
                      base_path,
                      font_path,
                      current_state);

        //add the player back into the reloaded state
        if (tilemap_state_t *reloaded = dynamic_cast<tilemap_state_t*>(states.at(current_state).get())) {
          //add the player back in
          reloaded->set_player(player);

          //if we were in debug mode keep the old debug camera location
          if (using_debug) {
            reloaded->set_camera(old_camera);
          }
        }
      }
    } else if (debug) {
      logger::log_info("reload not implemented for non tilemap state " + std::to_string(last_state));
    }
  }

  /**
   * Generate a new procedural map
   */
  void state_manager_t::new_swamp() {

    //player needed to have been loaded already
    if (tilemap_state_t *prev_tilemap = dynamic_cast<tilemap_state_t*>(states.at(current_state).get())) {

      if (prev_tilemap->is_procedural()) {
        //reset the previous procedural state
        states.at(current_state) = load_procedural_state(prev_tilemap->get_player(),
                                                          tile_dim,
                                                          camera,
                                                          renderer,
                                                          *this);
      } else {
        //add a new state
        states.push_back(load_procedural_state(prev_tilemap->get_player(),
                                              tile_dim,
                                              camera,
                                              renderer,
                                              *this));
        //update previous
        last_state = current_state;
        //update the current
        current_state = states.size() - 1;
      }
    }
  }

  /**
   * Toggle the debug camera (if applicable)
   */
  void state_manager_t::toggle_debug_camera() {
    if (current_state != TITLE_STATE) {
      states.at(current_state)->toggle_debug_camera();
    }
  }

  /**
   * Set the current level state
   * @param name the state type
   */
  void state_manager_t::set_state(size_t type) {
    //check if the state is not yet loaded
    if (type >= states.size()) {
      //lazily load the next state
      last_loaded++;

      //attempt to load from deferred cfg list
      if (debug) {
        logger::log_info("loading state " +
                         deferred_cfgs.at(last_loaded));
      }

      if (last_loaded < (int)deferred_cfgs.size()) {
        //load a new state (state_builder.h)
        load_tm_state(*this,
                      deferred_cfgs.at(last_loaded),
                      renderer,
                      camera,
                      tile_dim,
                      base_path,
                      font_path);

      } else {
        logger::log_err("no cfg provided for next level");
        return;
      }
    }

    //set player if previous and next states are tilemaps
    if (tilemap_state_t *src = dynamic_cast<tilemap_state_t*>(states.at(current_state).get())) {
      if (tilemap_state_t *dest = dynamic_cast<tilemap_state_t*>(states.at(type).get())) {

        //set player ptr
        dest->set_player(src->get_player());
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

    if (this->paused) {
      pause_state->handle_event(e);
    } else {
      states.at(current_state)->handle_event(e);
    }

    //check for pause button
    if ((e.type == SDL_KEYDOWN) &&
        (e.key.keysym.sym == SDLK_ESCAPE) &&
        (current_state != TITLE_STATE)) {
      this->paused = !this->paused;
    }

    //check for the free camera key
    if (debug &&
        (e.type == SDL_KEYDOWN) &&
        (e.key.keysym.sym == SDLK_v) &&
        (current_state != TITLE_STATE)) {

      //turn on the debug camera
      states.at(current_state)->toggle_debug_camera();
    }
  }

  /**
   * Add a state to the manager
   * @param s the state
   * @param idx_override whether to override the index
   */
  void state_manager_t::add_state(std::unique_ptr<state_t> s,
                                  int idx_override) {

    if ((idx_override >= 0) &&
        (idx_override < (int)states.size())) {

      //add at specific position
      states.at(idx_override) = std::move(s);

    } else {
      states.push_back(std::move(s));
    }
  }

  /**
   * Set the state paths in the manager but don't
   * load until the level is required
   * @param cfgs     the paths to the level configurations
   * @param base_path the resource directory base path
   * @param font_path the path to the font to use
   */
  void state_manager_t::load_defer(const std::vector<std::string>& cfgs,
                                   const std::string& base_path,
                                   const std::string& font_path) {
    //set the deferred states
    deferred_cfgs = cfgs;
    this->base_path = base_path;
    this->font_path = font_path;
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
   * Unpause the state (if paused)
   */
  void state_manager_t::unpause() {
    this->paused = false;
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

    if (!this->paused) {
      //update the state
      states.at(current_state)->update();
    }
  }

  /**
   * Render the current gamestate
   * @param renderer the renderer
   * @param debug    whether debug mode enabled
   */
  void state_manager_t::render(SDL_Renderer& renderer,
                               bool debug) {
    //lock the state
    std::unique_lock<std::shared_mutex> state_lock(lock);

    //render the state
    states.at(current_state)->render(renderer, debug);

    if (this->paused) {
      pause_state->render(renderer,debug);
    }
  }

  /**
   * Render any debug info
   * @param renderer sdl renderer
   * @param font     loaded ttf font
   */
  void state_manager_t::render_debug_info(SDL_Renderer& renderer,
                                           TTF_Font& font) {

    //lock the state
    std::unique_lock<std::shared_mutex> state_lock(lock);

    if (!this->paused) {
      states.at(current_state)->render_debug_info(renderer,font);
    }
  }
}}
