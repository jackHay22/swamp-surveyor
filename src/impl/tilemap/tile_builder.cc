/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "tile_builder.h"
#include <stdlib.h>

namespace impl {
namespace tilemap {
namespace tile_builder {


  /**
   * Make a ground tile
   * @param  tileset_constructor the tileset constructor
   * @param  s                   the slope of the tile
   * @return                     the tile type (id)
   */
  [[nodiscard]] int make_grnd_tile(tileset_constructor_t& tileset_constructor,
                                   tile_slope s) {
    //allocate a new tile
    int new_id = tileset_constructor.add_tile();

    //fill the entire tile
    tileset_constructor.fill_tile(
      new_id,
      DARK_GREEN_R,
      DARK_GREEN_G,
      DARK_GREEN_B
    );

    //get the tile dimension
    int dim = tileset_constructor.get_dim();

    if ((s == SLOPE_L) || (s == SLOPE_BOTH)) {
      //erase some pixels
      tileset_constructor.erase(new_id,0,0);
      tileset_constructor.erase(new_id,0,1);
      tileset_constructor.erase(new_id,0,2);
      tileset_constructor.erase(new_id,0,3);
      tileset_constructor.erase(new_id,1,0);
      tileset_constructor.erase(new_id,1,1);
      tileset_constructor.erase(new_id,2,0);
      tileset_constructor.erase(new_id,3,0);

      //TODO some randomness
    }

    if ((s == SLOPE_R) || (s == SLOPE_BOTH)) {
      //erase some pixels
      tileset_constructor.erase(new_id,dim-1,0);
      tileset_constructor.erase(new_id,dim-1,1);
      tileset_constructor.erase(new_id,dim-1,2);
      tileset_constructor.erase(new_id,dim-1,3);
      tileset_constructor.erase(new_id,dim-1,0);
      tileset_constructor.erase(new_id,dim-1,1);
      tileset_constructor.erase(new_id,dim-3,0);
      tileset_constructor.erase(new_id,dim-4,0);
    }

    return new_id;
  }

  /**
   * Make a non ground tile
   * @param  tileset_constructor the tileset constructor
   * @param  s                   the slope of the tile
   * @param  c                   the color profile for the tile
   * @return                     the tile type (id)
   */
  [[nodiscard]] int make_tile(tileset_constructor_t& tileset_constructor,
                              tile_slope s,
                              color_profile c) {
    return 0; //TODO
  }
}}}
