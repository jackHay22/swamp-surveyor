/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TITLE_STATE_H
#define _IO_JACKHAY_SWAMP_TITLE_STATE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "state.h"
#include "state_manager.h"
#include <string>

namespace impl {
namespace state {

  /**
   * The starting state displaying options
   */
  struct title_state_t : public state_t {
  private:
    int width;
    int height;

    //text width and height
    int text_w_start;
    int text_w_options;
    int text_w_quit;
    int text_h_start;
    int text_h_options;
    int text_h_quit;

    int caret_w;
    int caret_h;

    //background image texture
    SDL_Texture* texture = NULL;

    //the caret texture
    SDL_Texture* caret_texture = NULL;

    //menu option textures
    SDL_Texture* start_texture = NULL;
    SDL_Texture* options_texture = NULL;
    SDL_Texture* quit_texture = NULL;

    //the current menu option selected
    enum {
      START, OPTIONS, QUIT
    } selected_option = START;

    /**
     * Change the selection up
     */
    void select_up();

    /**
     * Change the selection down
     */
    void select_down();

    /**
     * Load text into a texture
     * @param text        the text to load
     * @param font_path   the path to the font to use
     * @param texture_ptr the texture ptr to set
     */
    void load_text_texture(const std::string& text,
                           const std::string& font_path,
                           SDL_Texture *texture_ptr);

  public:
    /**
     * Constructor
     * Throws resource exception
     * @param width    the width of the screen
     * @param height   the height of the screen
     * @param bg_path  the path to the background image
     * @param caret_path the path to the caret texture for current selection
     * @param font_path the path to the menu font
     * @param renderer the renderer for loading the texture
     * @param manager  the state manager
     */
    title_state_t(int width,
                  int height,
                  const std::string& bg_path,
                  const std::string& caret_path,
                  const std::string& font_path,
                  SDL_Renderer& renderer,
                  state_manager_t& manager);
    title_state_t(const title_state_t&) = delete;
    title_state_t& operator=(const title_state_t&) = delete;

    //destructor
    ~title_state_t();

    /**
     * Handle some key event
     * @param e the event
     */
    void handle_event(const SDL_Event& e);

    /**
     * Update this tile
     */
    void update();

    /**
     * Render the current gamestate
     * @param renderer the renderer
     */
    void render(SDL_Renderer& renderer) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_TITLE_STATE_H*/
