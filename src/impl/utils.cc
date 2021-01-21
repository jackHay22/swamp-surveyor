/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "utils.h"
#include "exceptions.h"
#include <SDL2/SDL_ttf.h>

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
                            int& w, int& h) {
    //load the image
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == NULL) {
      //failed to load
      throw exceptions::rsrc_exception_t(path);
    }

    //load to texture
    SDL_SetColorKey(surface,SDL_TRUE,SDL_MapRGB(surface->format,0,0xFF,0xFF));

    //use pixels to create texture
    SDL_Texture *texture = SDL_CreateTextureFromSurface(&renderer,surface);

    if(texture == NULL) {
      //throw an exception with sdl error detail
      throw exceptions::rsrc_exception_t(path,
                                         "could not create texture (" +
                                         std::string(SDL_GetError()) + ")");
    }

    //set the width and height
    w = surface->w;
    h = surface->h;

    //free surface
    SDL_FreeSurface(surface);
    return texture;
  }

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
                         SDL_Color color) {

    //load the font by path
    TTF_Font *font = TTF_OpenFont(font_path.c_str(), size);
    if (!font) {
      //failed to load the font
      throw exceptions::rsrc_exception_t(font_path,
                                         "could not load font (" +
                                         std::string(TTF_GetError()) + ")");
    }

    //load the text surface
    SDL_Surface* text_surface = TTF_RenderText_Solid(font,text.c_str(),color);
    if (!text_surface) {
      throw exceptions::rsrc_exception_t(font_path,
                                         "could not create text surface from font");
    }

    //create a texture from this surface
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(&renderer,text_surface);

    //set the width and height
    w = text_surface->w;
    h = text_surface->h;

    //free the original surface
    SDL_FreeSurface(text_surface);

    return text_texture;
  }
}}
