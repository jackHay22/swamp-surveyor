/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_UTILS_H
#define _IO_JACKHAY_SWAMP_UTILS_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <string>

namespace impl {
namespace utils {

  /**
   * Load a texture
   * Throws resource exception on failure
   * @param  path     the path to the texture
   * @param  renderer the renderer for loading the texture
   * @param  w        the width of the texture set
   * @param  h        the height of the texture set
   * @return          the loaded texture
   */
  SDL_Texture* load_texture(const std::string& path,
                            SDL_Renderer& renderer,
                            int& w, int& h);
}}

#endif /*_IO_JACKHAY_SWAMP_UTILS_H*/
