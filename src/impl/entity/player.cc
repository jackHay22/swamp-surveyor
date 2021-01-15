/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "player.h"
#include <iostream>

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
                     SDL_Renderer& renderer,
                     bool debug)
    : entity_t(x,y,w,h,anim_cfg_paths,renderer,debug)
    {}

  /**
   * Handle some key event
   * @param e the event
   */
  void player_t::handle_event(const SDL_Event& e) {
    //check if key is pressed or released
    if (e.type == SDL_KEYDOWN) {
      //check key pressed
      switch (e.key.keysym.sym) {
        //case SDLK_w: state = ; break;
        case SDLK_a: state = MOVE_LEFT; break;
        //case SDLK_s: state = ; break;
        case SDLK_d: state = MOVE_RIGHT; break;
      }
    } else if (e.type == SDL_KEYUP) {
      //check key released
      switch (e.key.keysym.sym) {
        //case SDLK_w: state = ; break;
        case SDLK_a: state = IDLE_LEFT; break;
        //case SDLK_s: state = ; break;
        case SDLK_d: state = IDLE_RIGHT; break;
      }
    }
  }

  /**
   * Update this entity at the tick
   */
  void player_t::update() {
    entity_t::update();
  }

  /**
   * Render the entity
   * @param renderer the renderer to use
   */
  void player_t::render(SDL_Renderer& renderer) const {
    entity_t::render(renderer);
  }
}}
