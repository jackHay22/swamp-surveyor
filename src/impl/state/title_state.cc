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
  #define FONT_SIZE 14

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
    start_texture = utils::load_font(START_TEXT,
                                     font_path,
                                     renderer,
                                     FONT_SIZE,
                                     text_w_start,
                                     text_h_start);
    options_texture = utils::load_font(OPTIONS_TEXT,
                                       font_path,
                                       renderer,
                                       FONT_SIZE,
                                       text_w_options,
                                       text_h_options);
    quit_texture = utils::load_font(QUIT_TEXT,
                                    font_path,
                                    renderer,
                                    FONT_SIZE,
                                    text_w_quit,
                                    text_h_quit);
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
    if (e.type == SDL_KEYDOWN) {
      //switch on the key type
      switch (e.key.keysym.sym) {
        //case SDLK_w: state = ; break;
        case SDLK_RETURN:
          //TODO current option
          manager.next_level_state();
          break;
      }
    }
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

    int x_offset = this->width / 6;
    int y_offset = this->height / 4;

    //render text options
    SDL_Rect sample_bounds = {0,0,text_w_start,text_h_start};
    SDL_Rect text_bounds = {x_offset,
                            y_offset,
                            text_w_start,text_h_start};

    //render the start texture
    SDL_RenderCopy(&renderer,
                   this->start_texture,
                   &sample_bounds,
                   &text_bounds);

    //add vertical spacing
    y_offset += text_h_start;

    sample_bounds = {0,0,text_w_options,text_h_options};
    text_bounds = {x_offset,
                   y_offset,
                   text_w_options,text_h_options};

    //render the options texture
    SDL_RenderCopy(&renderer,
                  this->options_texture,
                  &sample_bounds,
                  &text_bounds);

    //add vertical spacing
    y_offset += text_h_start;

    sample_bounds = {0,0,text_w_quit,text_h_quit};
    text_bounds = {x_offset,
                   y_offset,
                   text_w_quit,text_h_quit};

    //render the quit texture
    SDL_RenderCopy(&renderer,
                   this->quit_texture,
                   &sample_bounds,
                   &text_bounds);

    //render caret
  }
}}
