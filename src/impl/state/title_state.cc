/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "title_state.h"
#include "../utils.h"

namespace impl {
namespace state {

  /**
   * Constructor
   * Throws resource exception
   * @param width    the width of the screen
   * @param height   the height of the screen
   * @param bg_path  the path to the background image
   * @param renderer the renderer for loading the texture
   */
  title_state_t::title_state_t(int width,
                               int height,
                               const std::string& bg_path,
                               SDL_Renderer& renderer)
    : state_t(SDL_Rect {0,0,width,height}),
      width(width),
      height(height) {

    int w,h = 0;
    //load the background texture
    this->texture = utils::load_texture(bg_path,renderer,w,h);
  }

  /**
   * Handle some key event
   * @param e the event
   */
  void title_state_t::handle_event(const SDL_Event& e) {

  }

  /**
   * Update this tile
   */
  void title_state_t::update() {

  }

  /**
   * Render the current gamestate
   * @param renderer the renderer
   */
  void title_state_t::render(SDL_Renderer& renderer) const {

  }
}}
