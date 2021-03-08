/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "pause_state.h"
#include "../ui/button.h"

namespace impl {
namespace state {

  /**
   * Constructor
   * @param width    the window width
   * @param height   the window height
   * @param manager  the state manager
   */
  pause_state_t::pause_state_t(int width,
                               int height,
                               int window_scale,
                               state_manager_t& manager,
                               const std::string& font_path,
                               SDL_Renderer& renderer)
    : state_t(manager, SDL_Rect {0,0,width,height}) {
    //initialize the pause window
    this->window = std::make_unique<ui::window_t>(
      80,25,(width - 160), (height - 50),
      window_scale,
      manager
    );

    //Add buttons to the window
    this->window->add_component(
      std::make_unique<ui::button_t>("Settings",
        [](state_manager_t& m){

        },
        font_path,renderer)
    );
    this->window->add_component(
      std::make_unique<ui::button_t>("Map",
        [](state_manager_t& m){

        },
        font_path,renderer)
    );
    this->window->add_component(
      std::make_unique<ui::button_t>("Unpause",
        [](state_manager_t& m){

        },
        font_path,renderer)
    );
  }

  /**
   * Handle some key event
   * @param e the event
   */
  void pause_state_t::handle_event(const SDL_Event& e) {
    this->window->handle_event(e);
  }

  /**
   * Render the current gamestate
   * @param renderer the renderer
   * @param debug    whether debug mode enabled
   */
  void pause_state_t::render(SDL_Renderer& renderer, bool /*debug*/) const {
    this->window->render(renderer);
  }
}}
