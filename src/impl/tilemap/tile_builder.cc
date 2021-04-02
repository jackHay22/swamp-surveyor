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

  bool probably() {
    return (rand() % 3 >= 1);
  }

  bool maybe() {
    return (rand() % 4 == 1);
  }

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

    //edit the new tile
    edit_grnd_tile(tileset_constructor,new_id,s);

    return new_id;
  }

  /**
   * Make a non collidable tile that eases height differences
   * @param  tileset_constructor tileset constructor
   * @param  s                   slope direction
   * @return                     the tile id
   */
  [[nodiscard]] int make_low_slope_grnd_tile(tileset_constructor_t& tileset_constructor,
                                             tile_slope s) {
    int new_id = tileset_constructor.add_tile();
    int dim = tileset_constructor.get_dim();

    //set the color
    tileset_constructor.set_default_color(
      DARK_GREEN_R,
      DARK_GREEN_G,
      DARK_GREEN_B
    );

    //draw the bottom line
    tileset_constructor.draw_line(new_id,0,dim-1,dim-1,dim-1);

    //change in endpt position
    int d1 = 0;
    int d2 = 0;

    for (int i=0; i<(dim-4); i++) {
      //draw row
      tileset_constructor.draw_line(new_id,d1,dim-1-i,dim - d2,dim-1-i);

      if (s == SLOPE_R) {
        d2 += 2;
      } else if (s == SLOPE_L) {
        d1 += 2;
      }
    }
    return new_id;
  }

  /**
   * Edit the slope of a ground tile
   * @param tileset_constructor the tileset constructor
   * @param id                  the id of the tile to edit
   * @param s                   the slope to edit to
   */
  void edit_grnd_tile(tileset_constructor_t& tileset_constructor,
                      int id,
                      tile_slope s) {
    //get the tile dimension
    int dim = tileset_constructor.get_dim();

    if ((s == SLOPE_L) || (s == SLOPE_BOTH)) {
      //always erase
      tileset_constructor.erase(id,0,0);
      tileset_constructor.erase(id,0,1);
      tileset_constructor.erase(id,1,0);

      //probably erase
      if (probably()) {
        tileset_constructor.erase(id,0,2);
      }
      if (probably()) {
        tileset_constructor.erase(id,1,1);
      }
      if (probably()) {
        tileset_constructor.erase(id,2,0);
      }

      //maybe erase
      if (maybe()) {
        tileset_constructor.erase(id,0,3);
      }
      if (maybe()) {
        tileset_constructor.erase(id,3,0);
      }

      //other erases

      //TODO some randomness
    }

    if ((s == SLOPE_R) || (s == SLOPE_BOTH)) {
      //erase some pixels
      tileset_constructor.erase(id,dim-1,0);
      tileset_constructor.erase(id,dim-1,1);
      tileset_constructor.erase(id,dim-1,2);
      tileset_constructor.erase(id,dim-1,3);
      tileset_constructor.erase(id,dim-2,0);
      tileset_constructor.erase(id,dim-2,1);
      tileset_constructor.erase(id,dim-3,0);
      tileset_constructor.erase(id,dim-4,0);
    }
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
