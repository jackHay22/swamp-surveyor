/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "chemical_label.h"

namespace impl {
namespace items {

  /**
   * Constructor
   * @param x            position of the item x
   * @param y            position of the item y
   * @param texture_path the path to the texture
   * @param renderer     the renderer for loading the texture
   * @param debug        whether debug mode enabled
   */
  chemical_label_t::chemical_label_t(int x, int y,
                                     const std::string& texture_path,
                                     SDL_Renderer& renderer,
                                     bool debug)
    : item_t(x,y,texture_path,renderer,debug) {}
}}
