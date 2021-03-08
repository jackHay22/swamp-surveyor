/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "window.h"
#include <iostream>

namespace impl {
namespace ui {

  #define R_BORDER 176
  #define G_BORDER 191
  #define B_BORDER 178
  #define R_FILL 143
  #define G_FILL 166
  #define B_FILL 146
  #define R_HIGHLIGHT 210
  #define G_HIGHLIGHT 217
  #define B_HIGHLIGHT 211

  /**
   * Constructor
   * @param x position x
   * @param y position y
   * @param w width
   * @param h height
   * @param window_scale the scale of the window
   */
  window_t::window_t(int x, int y, int w, int h, int window_scale)
    : x(x), y(y),
      w(w), h(h),
      cursor_x(0),
      cursor_y(0),
      window_scale(window_scale),
      subcomponents() {}

  /**
   * Whether a given x y position is in bounds
   * @param  x position x
   * @param  y position y
   * @return   whether this position is in bounds
   */
  bool window_t::in_bounds(int x, int y) const {
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
  SDL_Rect window_t::bounds() const {
    return {x,y,w,h};
  }

  /**
   * Handle an SDL event (to calculate mouse position)
   */
  void window_t::handle_event(const SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION) {
     int mx = 0;
     int my = 0;
     SDL_GetMouseState(&mx,&my);

     //scale the position
     cursor_x = mx / window_scale;
     cursor_y = my / window_scale;
    } else if ((e.type == SDL_MOUSEBUTTONDOWN) &&
              (e.button.button == SDL_BUTTON_LEFT)) {

      //check if the cursor is currently in the window
      if (in_bounds(cursor_x,cursor_y)) {
        //click any components under cursor
        for (size_t i=0; i<subcomponents.size(); i++) {
          if (subcomponents.at(i).in_bounds(cursor_x, cursor_y)) {
            subcomponents.at(i).clicked();
          }
        }
      }
    }
  }

  /**
   * Render the var
   * @param renderer the renderer to use
   */
  void window_t::render(SDL_Renderer& renderer) const {
    //set the draw color
    SDL_SetRenderDrawColor(&renderer,R_BORDER,G_BORDER,B_BORDER,255);

    //the rectangle around the bar
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
    SDL_RenderDrawLine(&renderer,x,y,x + w - 2,y);

    SDL_RenderDrawLine(&renderer,x,y,x,y + h - 1);

    // Render the cursor
    SDL_SetRenderDrawColor(&renderer,0,0,0,225);
    SDL_RenderDrawPoint(&renderer,cursor_x,cursor_y);
    SDL_RenderDrawPoint(&renderer,cursor_x - 1,cursor_y);
    SDL_RenderDrawPoint(&renderer,cursor_x - 1,cursor_y - 1);
    SDL_RenderDrawPoint(&renderer,cursor_x,cursor_y - 1);
    SDL_RenderDrawPoint(&renderer,cursor_x - 1,cursor_y - 2);
    SDL_RenderDrawPoint(&renderer,cursor_x - 1,cursor_y + 1);
    SDL_RenderDrawPoint(&renderer,cursor_x + 1,cursor_y - 1);
    SDL_RenderDrawPoint(&renderer,cursor_x + 1,cursor_y + 1);
  }
}}
