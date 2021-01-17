/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_ENVIRONMENT_BUILDER_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_ENVIRONMENT_BUILDER_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <string>
#include "renderable.h"

namespace impl {
namespace environment {

  /**
   * Load renderable environmental elements
   * @param elems    a vector of loaded elements set by the call
   * @param cfg_path the path to the environment configuration
   * @param renderer the sdl renderer
   * @param debug    whether debug mode is enabled
   */
  void load_env_elems(std::vector<std::shared_ptr<environment::renderable_t>>& elems,
                      const std::string& cfg_path,
                      SDL_Renderer& renderer,
                      bool debug);
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_ENVIRONMENT_BUILDER_H*/
