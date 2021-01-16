/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "title_state.h"
#include "../utils.h"
#include "../exceptions.h"

namespace impl {
namespace state {

  #define START_TEXT "Start"
  #define OPTIONS_TEXT "Options"
  #define QUIT_TEXT "Quit"
  #define FONT_SIZE 12

  /**
   * Constructor
   * Throws resource exception
   * @param width    the width of the screen
   * @param height   the height of the screen
   * @param bg_path  the path to the background image
   * @param font_path the path to the menu font
   * @param renderer the renderer for loading the texture
   * @param manager  the state manager
   */
  title_state_t::title_state_t(int width,
                               int height,
                               const std::string& bg_path,
                               const std::string& font_path,
                               SDL_Renderer& renderer,
                               state_manager_t& manager)
    : state_t(manager, SDL_Rect {0,0,width,height}),
      width(width),
      height(height) {

    int w,h = 0;
    //load the background texture
    this->texture = utils::load_texture(bg_path,renderer,w,h);

    //load the font textures
    start_texture = utils::load_font(START_TEXT,font_path,renderer,FONT_SIZE);
    options_texture = utils::load_font(OPTIONS_TEXT,font_path,renderer,FONT_SIZE);
    quit_texture = utils::load_font(QUIT_TEXT,font_path,renderer,FONT_SIZE);
  }

  //destructor
  title_state_t::~title_state_t() {
    //destroy the bg texture
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
    if (start_texture != NULL) {
      SDL_DestroyTexture(start_texture);
    }
    if (options_texture != NULL) {
      SDL_DestroyTexture(options_texture);
    }
    if (quit_texture != NULL) {
      SDL_DestroyTexture(quit_texture);
    }
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
    //bounds
    SDL_Rect image_bounds = {0,0,this->width, this->height};

    //render the background texture
    SDL_RenderCopy(&renderer,
                   this->texture,
                   &image_bounds,
                   &image_bounds);

    int x_offset = this->width / 3;

    //render text options
  }
}}
