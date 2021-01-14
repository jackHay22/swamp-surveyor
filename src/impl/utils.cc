/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "utils.h"
#include "exceptions.h"

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
}}
