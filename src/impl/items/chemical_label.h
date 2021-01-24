/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_CHEMICAL_LABEL_H
#define _IO_JACKHAY_SWAMP_CHEMICAL_LABEL_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <string>
#include "item.h"

namespace impl {
namespace items {

  /**
   * A chemical label that can be picked up
   * by the player
   */
  struct chemical_label_t : public item_t {

  public:
    /**
     * Constructor
     * @param x            position of the item x
     * @param y            position of the item y
     * @param texture_path the path to the texture
     * @param renderer     the renderer for loading the texture
     */
    chemical_label_t(int x, int y,
                     const std::string& texture_path,
                     SDL_Renderer& renderer)
      :  item_t(x,y,texture_path,renderer) {}
    chemical_label_t(const chemical_label_t&) = delete;
    chemical_label_t& operator=(const chemical_label_t&) = delete;

  };
}}

#endif /*_IO_JACKHAY_SWAMP_CHEMICAL_LABEL_H*/
