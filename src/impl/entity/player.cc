/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "player.h"

namespace impl {
namespace entity {

  /**
   * Constructor
   * @param x                  entity position x (center)
   * @param y                  entity position y (center)
   * @param w                  entity bounds width
   * @param h                  entity bounds height
   * @param texture_paths      paths for each of the 4 basic animations
   * @param renderer           the renderer for loading textures
   */
  player_t::player_t(int x, int y,
           int w, int h,
           const std::vector<std::string>& anim_cfg_paths,
           SDL_Renderer& renderer)
    : entity_t(x,y,w,h,anim_cfg_paths,renderer)
    {}

  /**
   * Handle some key event
   * @param e the event
   */
  void player_t::handle_event(const SDL_Event& e) {

  }

  /**
   * Update this entity at the tick
   */
  void player_t::update() {

  }

  /**
   * Render the entity
   * @param renderer the renderer to use
   */
  void player_t::render(SDL_Renderer& renderer) const {

  }
}}
