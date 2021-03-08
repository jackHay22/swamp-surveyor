/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "button.h"
#include "colors.h"
#include "../utils.h"

namespace impl {
namespace ui {

  #define FONT_SIZE 10

  /**
   * Constructor
   * @param text the button text
   * @param window_scale the scale of the window
   * @param font_path the path to the font to use
   * @param renderer the renderer to load the font
   */
  button_t::button_t(const std::string& text,
                     std::function<void(state::state_manager_t& manager)> handler,
                     const std::string& font_path,
                     SDL_Renderer& renderer)
    : x(0), y(0), w(0), h(0),
      texture(NULL), texture_w(0), texture_h(0),
      was_clicked(false), handler(handler) {
    //load the texture
    texture = utils::load_font(text,
                               font_path,
                               renderer,
                               FONT_SIZE,
                               texture_w,
                               texture_h,
                               {255,255,255});
  }

  /**
   * Destructor to free texture
   */
  button_t::~button_t() {
    //free the texture
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }

  /**
   * Called when a component is clicked
   */
  void button_t::clicked(state::state_manager_t& manager) {
    handler(manager);
    was_clicked = true;
  }

  /**
   * Called when the component is unclicked
   */
  void button_t::unclicked() {
    was_clicked = false;
  }

  /**
   * Set the component attributes
   * @param x position x
   * @param y position y
   * @param w dimension w
   * @param h dimension h
   */
  void button_t::set_attributes(int x, int y, int w, int h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
  }

  /**
   * Whether a given x y position is in bounds
   * @param  x position x
   * @param  y position y
   * @return   whether this position is in bounds
   */
  bool button_t::in_bounds(int x, int y) const {
    //check if in bounds
    return (x > this->x) &&
           (x < (this->x + w)) &&
           (y > this->y) &&
           (y < (this->y + h));
  }

  /**
   * Get the bounds of this component
   * @return the bounds of the component on the screen
   */
  SDL_Rect button_t::bounds() const {
    return {x,y,w,h};
  }

  /**
   * Handle an SDL event (to calculate mouse position)
   */
  void button_t::handle_event(const SDL_Event& /*e*/) { }

  /**
   * Render the var
   * @param renderer the renderer to use
   */
  void button_t::render(SDL_Renderer& renderer) const {

    if (was_clicked) {
      //render the button
      SDL_SetRenderDrawColor(&renderer,R_DARK,G_DARK,B_DARK,255);

      SDL_Rect rect = {x,y,w,h};

      //render the bounds
      SDL_RenderDrawRect(&renderer,&rect);

      //set the draw color
      SDL_SetRenderDrawColor(&renderer,R_FILL,G_FILL,B_FILL,255);

      SDL_Rect fill = {x+1,y+1,w-2,h-2};

      //render the fill
      SDL_RenderFillRect(&renderer,&fill);

      SDL_SetRenderDrawColor(&renderer,R_HIGHLIGHT,G_HIGHLIGHT,B_HIGHLIGHT,255);

      //render the highlight
      //vert
      SDL_RenderDrawLine(&renderer,x+1,y+h-1,x+w-2,y+h-1);

      //horiz
      SDL_RenderDrawLine(&renderer,x+w-1,y+1,x+w-1,y+h-1);

    } else {
      //render the button
      SDL_SetRenderDrawColor(&renderer,R_DARK,G_DARK,B_DARK,255);

      SDL_Rect rect = {x,y,w,h};

      //render the bounds
      SDL_RenderDrawRect(&renderer,&rect);

      //set the draw color
      SDL_SetRenderDrawColor(&renderer,R_BORDER,G_BORDER,B_BORDER,255);

      SDL_Rect fill = {x+1,y+1,w-2,h-2};

      //render the fill
      SDL_RenderFillRect(&renderer,&fill);

      SDL_SetRenderDrawColor(&renderer,R_HIGHLIGHT,G_HIGHLIGHT,B_HIGHLIGHT,255);

      //render the highlight
      SDL_RenderDrawLine(&renderer,x,y,x + w - 2,y);

      SDL_RenderDrawLine(&renderer,x,y,x,y + h - 1);
    }

    //the y position depends on the click state
    int y_pos = y - 1;
    if (was_clicked) {
      y_pos++;
    }

    //text x position
    int x_pos = this->x + (this->w / 2) - (texture_w / 2);

    SDL_Rect sample_bounds = {0,0,texture_w,texture_h};
    SDL_Rect text_bounds = {x_pos,y_pos,texture_w,texture_h};

    //render the texture
    SDL_RenderCopy(&renderer,
                   texture,
                   &sample_bounds,
                   &text_bounds);
  }
}}
