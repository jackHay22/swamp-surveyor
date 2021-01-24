/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_SPRAYER_H
#define _IO_JACKHAY_SWAMP_SPRAYER_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <string>
#include "item.h"

namespace impl {
namespace items {

  /**
   * A sprayer item the player needs to
   * disperse foam
   */
  struct sprayer_t : public item_t {

  public:
    /**
     * Constructor
     * @param x            position of the item x
     * @param y            position of the item y
     * @param texture_path the path to the texture
     * @param renderer     the renderer for loading the texture
     */
    sprayer_t(int x, int y,
               const std::string& texture_path,
               SDL_Renderer& renderer)
       : item_t(x,y,texture_path,renderer) {}
    sprayer_t(const sprayer_t&) = delete;
    sprayer_t& operator=(const sprayer_t&) = delete;

  };
}}

#endif /*_IO_JACKHAY_SWAMP_SPRAYER_H*/
