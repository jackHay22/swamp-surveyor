/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TILEMAP_TILEBUILDER_H
#define _IO_JACKHAY_SWAMP_TILEMAP_TILEBUILDER_H

#include "tileset_constructor.h"

namespace impl {
namespace tilemap {
namespace tile_builder {

  //COLORS
  #define LIGHT_GREEN_R 125
  #define LIGHT_GREEN_G 174
  #define LIGHT_GREEN_B 109
  #define MED_LIGHT_GREEN_R 104
  #define MED_LIGHT_GREEN_G 156
  #define MED_LIGHT_GREEN_B 91
  #define MED_DARK_GREEN_R 72
  #define MED_DARK_GREEN_G 110
  #define MED_DARK_GREEN_B 67
  #define DARK_GREEN_R 41
  #define DARK_GREEN_G 58
  #define DARK_GREEN_B 0
  #define DARK_R 8
  #define DARK_G 17
  #define DARK_B 0

  /*
   * The slope of a ground tile
   */
  enum tile_slope {
    FLAT,
    SLOPE_L,
    SLOPE_R,
    SLOPE_BOTH
  };

  /*
   * The primary color profile for a tile
   */
  enum color_profile {
    BACKGRND,         // a background tile, low color detail
    FOREGRND,         // a foreground tile, low color detail
    NEARBG_DETAIL     // a near background tile with color detail
  };

  /**
   * Make a ground tile
   * @param  tileset_constructor the tileset constructor
   * @param  s                   the slope of the tile
   * @return                     the tile type (id)
   */
  [[nodiscard]] int make_grnd_tile(tileset_constructor_t& tileset_constructor,
                                   tile_slope s);

  /**
   * Make a non collidable tile that eases height differences
   * @param  tileset_constructor tileset constructor
   * @param  s                   slope direction
   * @return                     the tile id
   */
  [[nodiscard]] int make_low_slope_grnd_tile(tileset_constructor_t& tileset_constructor,
                                             tile_slope s);

  /**
   * Make a tile that adds roughness to flat ground
   * @param  tileset_constructor the tileset constuctor
   * @return                     the tileid
   */
  [[nodiscard]] int make_flat_grnd_tile(tileset_constructor_t& tileset_constructor);

  /**
   * Edit the slope of a ground tile
   * @param tileset_constructor the tileset constructor
   * @param id                  the id of the tile to edit
   * @param s                   the slope to edit to
   */
  void edit_grnd_tile(tileset_constructor_t& tileset_constructor,
                      int id,
                      tile_slope s);

  /**
   * Make a non ground tile
   * @param  tileset_constructor the tileset constructor
   * @param  s                   the slope of the tile
   * @param  c                   the color profile for the tile
   * @return                     the tile type (id)
   */
  [[nodiscard]] int make_tile(tileset_constructor_t& tileset_constructor,
                              tile_slope s,
                              color_profile c);
}}}

#endif /*_IO_JACKHAY_SWAMP_TILEMAP_TILEBUILDER_H*/
