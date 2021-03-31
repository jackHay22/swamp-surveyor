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
#include "tilemap_state.h"
#include "../entity/player.h"

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
   * @param  base_path resource directory base path
   * @param  font_path the path to the font to use
   * @param  idx_override put the state in a specific position
   */
  void load_tm_state(state::state_manager_t& state_manager,
                     const std::string& path,
                     SDL_Renderer& renderer,
                     SDL_Rect& camera,
                     int tile_dim,
                     const std::string& base_path,
                     const std::string& font_path,
                     int idx_override=-1);

  /**
   * Load a new procedural tilemap state
   * @param  player   the previous player
   * @param  tile_dim the tile dimension
   * @param  camera   the camera to use
   * @param  renderer the renderer for creating textures
   * @param  manager  the state manager
   * @return          the new state
   */
  std::unique_ptr<state_t> load_procedural_state(std::shared_ptr<entity::player_t> player,
                                                 int tile_dim,
                                                 SDL_Rect& camera,
                                                 SDL_Renderer& renderer,
                                                 state_manager_t& manager);
}}

#endif /*_IO_JACKHAY_SWAMP_STATE_BUILDER_H*/
