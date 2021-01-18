/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_INDICATOR_BAR_H
#define _IO_JACKHAY_SWAMP_INDICATOR_BAR_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

namespace impl {
namespace entity {

  /**
   * A bar that indicates some level (i.e. health)
   */
  struct indicator_bar_t {
  private:
    //the position and width of the bar
    //height is 3 px by default
    int x;
    int y;
    int w;
    //the maximum value this bar can be
    int max_val;
    //the current value of this bar
    int curr_val;

    //the color of the indicator
    int r;
    int g;
    int b;

  public:
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
    indicator_bar_t(int x,
                    int y,
                    int w,
                    int max_val,
                    int r, int g, int b);
    indicator_bar_t(const indicator_bar_t&) = delete;
    indicator_bar_t& operator=(const indicator_bar_t&) = delete;

    /**
     * Set the value that this bar indicates
     */
    void set_val(int curr_val) { this->curr_val = curr_val; }

    /**
     * Render the var
     * @param renderer the renderer to use
     */
    void render(SDL_Renderer& renderer) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_INDICATOR_BAR_H*/
