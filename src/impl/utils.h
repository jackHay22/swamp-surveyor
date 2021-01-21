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

  /**
   * Load a font to a texture
   * Throws resource exception on failure
   * @param  text      the text
   * @param  font_path the path to the font to load
   * @param  renderer  the renderer
   * @param  size      the size of the font
   * @param  w         the width of the loaded texture set by the call
   * @param  h         the height of the loaded texture set by the call
   * @param  color     the color for the text
   * @return           the loaded texture
   */
  SDL_Texture* load_font(const std::string& text,
                         const std::string& font_path,
                         SDL_Renderer& renderer,
                         int size, int& w, int& h,
                         SDL_Color color = {0,0,0});
}}

#endif /*_IO_JACKHAY_SWAMP_UTILS_H*/
