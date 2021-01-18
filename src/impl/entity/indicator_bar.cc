/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "indicator_bar.h"

namespace impl {
namespace entity {

  //the height in px of the indicator bar
  #define BAR_HEIGHT 3

  /**
   * Constructor
   * @param x       position of the bar x
   * @param y       position of the bar y
   * @param w       the graphical width of the bar
   * @param max_val the max value of the bar range (starts at this value)
   * @param r       the color r
   * @param g       the color g
   * @param b       the color b
   */
  indicator_bar_t::indicator_bar_t(int x,
                                   int y,
                                   int w,
                                   int max_val,
                                   int r, int g, int b)
    : x(x), y(y), w(w),
      max_val(max_val),
      curr_val(max_val),
      r(r), g(g), b(b) {}

  /**
   * Render the var
   * @param renderer the renderer to use
   */
  void indicator_bar_t::render(SDL_Renderer& renderer) const {
    //set the draw color
    SDL_SetRenderDrawColor(&renderer,r,g,b,255);

    //the rectangle around the bar
    SDL_Rect rect = {x,y,w,BAR_HEIGHT};

    //render the bounds
    SDL_RenderDrawRect(&renderer,&rect);

    //map the current value to the bar size
    int bar_len = ((float) curr_val / (float) max_val) * (w - 2);

    //render the progress bar
    SDL_RenderDrawLine(&renderer,
                       x + 1,
                       y + 1,
                       x + bar_len,
                       y + 1);
  }
}}
