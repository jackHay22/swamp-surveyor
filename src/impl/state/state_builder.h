/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_STATE_BUILDER_H
#define _IO_JACKHAY_SWAMP_STATE_BUILDER_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <string>
#include "state.h"
#include "state_manager.h"

namespace impl {
namespace state {

  /**
   * Load a state from a configuration
   * Throws a resource exception if load fails
   * @param  state_manager  the manager to load new states into
   * @param  path     the path to the level configuration
   * @param  renderer the renderer for the level
   * @param  camera   the starting level camera
   * @param  tile_dim the dimensions of tiles
   * @param  debug    whether debug mode enabled
   */
  void load_tm_state(state::state_manager_t& state_manager,
                     const std::string& path,
                     SDL_Renderer& renderer,
                     SDL_Rect& camera,
                     int tile_dim,
                     bool debug);
}}

#endif /*_IO_JACKHAY_SWAMP_STATE_BUILDER_H*/
