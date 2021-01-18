/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ITEMS_ITEM_BUILDER_H
#define _IO_JACKHAY_SWAMP_ITEMS_ITEM_BUILDER_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <string>
#include "item.h"

namespace impl {
namespace items {

  /**
   * Load items from cfg
   * @param items    a vector of loaded items set by the call
   * @param cfg_path the path to the items configuration
   * @param renderer the sdl renderer
   * @param debug    whether debug mode is enabled
   */
  void load_items(std::vector<std::shared_ptr<item_t>>& items,
                    const std::string& cfg_path,
                    SDL_Renderer& renderer,
                    bool debug);
}}

#endif /*_IO_JACKHAY_SWAMP_ITEMS_ITEM_BUILDER_H*/
