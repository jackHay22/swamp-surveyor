/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "state.h"
#include "state_manager.h"

namespace impl {
namespace state {

  //constructor
  state_t::state_t(state_manager_t& manager,
          SDL_Rect camera)
    : _camera(camera),
      _debug_camera(camera),
      manager(manager),
      using_debug(false) {}

  /**
   * Get the debug camera
   * @return the debug camera
   */
  SDL_Rect& state_t::get_debug_camera() {
    return _debug_camera;
  }

  /**
   * Get the active camera
   * @return the camera
   */
  const SDL_Rect& state_t::get_active_camera() const {
    if (using_debug) {
      return _debug_camera;
    }
    return _camera;
  }

  /**
   * Get the player camera
   * @return the camera
   */
  SDL_Rect& state_t::get_player_camera() {
    return _camera;
  }

  /**
   * Toggle the debug camera
   */
  void state_t::toggle_debug_camera() {
    if (!using_debug) {
      //align the debug camera
      //(Now enabled)
      _debug_camera = _camera;
    }
    using_debug = !using_debug;
  }
}}
