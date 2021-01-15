/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "update_tile.h"

namespace impl {
namespace tilemap {

  /**
   * Constructor
   * @param x           position of tile x
   * @param y           position of tile y
   * @param dim         dimension of the tile
   * @param range_start start of the animation range
   * @param range_end   end of the animation range
   */
  update_tile_t::update_tile_t(int x, int y,
                               int dim, int range_start,
                               int range_end)
    : tile_t(x,y,dim,range_start,false),
      range_start(range_start),
      range_end(range_end) {}

  /**
   * Update this tile
   */
  void update_tile_t::update() {

  }
}}
