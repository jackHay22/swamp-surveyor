/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENTITY_BUILDER_H
#define _IO_JACKHAY_SWAMP_ENTITY_BUILDER_H

#include <memory>
#include <string>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "entity.h"

namespace impl {
namespace entity {

  /**
   * Load an entity from a cfg file
   * Throws rsrc_exception_t
   * @param  path the path to the cfg file
   * @param  renderer the renderer for loading textures
   * @param  tile_dim the dimension of tiles
   * @param  debug    whether debug mode enabled
   * @return      the entity
   */
  std::shared_ptr<entity_t> load_entity(const std::string& path,
                                        SDL_Renderer& renderer,
                                        int tile_dim,
                                        bool debug);
}}

#endif /*_IO_JACKHAY_SWAMP_ENTITY_BUILDER_H*/
