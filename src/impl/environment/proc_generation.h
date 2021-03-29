/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_PROC_GENERATION_H
#define _IO_JACKHAY_SWAMP_PROC_GENERATION_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <tuple>
#include <vector>
#include <string>
#include "texture_constructor.h"

namespace impl {
namespace environment {
namespace proc_generation {

  /**
   * Execute a fractal plant l system on this texture constructor
   * @param constructor the texture constructor
   * @param iters       the number of iterations to evaluate for
   * @param r           color r
   * @param g           color g
   * @param b           color b
   * @param x           position x
   * @param y           position y
   */
  void fractal_tree_lsystem(texture_constructor_t& constructor,
                            int iters,
                            int r, int g, int b,
                            int x, int y);

  /**
   * Generate trunk foliage given a pre constructed trunk texture
   *
   * - The presence of a pixel belonging to the trunk increases
   *   the liklihood that a leaf will be placed in the vicinity
   *
   *
   * @param constructor the constructor that already has a trunk
   * @param count       the number of leaves (density)
   * @param r           leaf color r
   * @param g           leaf color g
   * @param b           leaf color b
   * @param two_color   whether to use two colors for leaves
   * @param r2          leaf color r (second)
   * @param g2          leaf color g (second)
   * @param b2          leaf color b (second)
   */
  void trunk_foliage(texture_constructor_t& constructor,
                     int count,
                     int r, int g, int b,
                     bool two_color,
                     int r2, int g2, int b2);

  /**
   * Build a tree trunk by branching and conserving "volume"
   * @param constructor texture constructor
   * @param x,y position
   * @param r,g,b the color of the trunk
   */
  void branching_tree_growth(texture_constructor_t& constructor,
                             int x, int y,
                             int r, int g, int b);

}}}

#endif /*_IO_JACKHAY_SWAMP_PROC_GENERATION_H*/
