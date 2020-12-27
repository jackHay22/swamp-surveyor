/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_UPDATE_TILE_H
#define _IO_JACKHAY_SWAMP_UPDATE_TILE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <memory>
#include "tile.h"

namespace impl {
namespace tilemap {

  /**
   * Defines a tile in the map
   */
  struct update_tile_t : public tile_t {
  private:
    //the range of animation indices
    int range_start;
    int range_end;

  public:
    //constructor
    update_tile_t(int x, int y,
                  int dim, int range_start,
                  int range_end);
    update_tile_t(const update_tile_t&) = delete;
    update_tile_t& operator=(const update_tile_t&) = delete;

    /**
     * Update this tile
     */
    void update();
  };
}}

#endif /*_IO_JACKHAY_SWAMP_UPDATE_TILE_H*/
