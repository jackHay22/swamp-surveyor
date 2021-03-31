/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "procedural_tilemap.h"
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <cmath>
#include "noise.h"
#include <iostream>

namespace impl {
namespace tilemap {

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

  //the minimum terrain index in the tilemap (headroom)
  #define TERRAIN_MIN_IDX 10
  #define MAX_TILESET_WIDTH 64

  /**
   * Clamp a value
   * @param min  the min value (incl)
   * @param  val the value
   * @param  max the max
   * @return     clamped [min,max)
   */
  size_t clamp(size_t min, int val, size_t max) {
    if (val < (int)min) {
      return min;
    }
    if (val >= (int)max) {
      return max - 1;
    }
    return val;
  }

  /**
   * Constructor
   * @param dim      tile dimension
   * @param width_p  map width in pixels
   * @param height_p map height in pixels
   */
  procedural_tilemap_t::procedural_tilemap_t(int dim, int width_p, int height_p, SDL_Renderer& renderer)
    : dim(dim),
      width_p(width_p),
      height_p(height_p),
      tiles() {
    //generate terrain procedurally
    this->generate_terrain(renderer);
  }

  /**
   * Called by the constructor to generate procedural terrain
   * @param renderer the renderer for generating textures
   */
  void procedural_tilemap_t::generate_terrain(SDL_Renderer& renderer) {
    //use tiles for the ground only, the rest done in other ways
    size_t tiles_across = width_p / dim;
    size_t tiles_down = height_p / dim;

    tiles.reserve(tiles_down);

    for (size_t r=0; r<tiles_down; r++) {
      tiles.emplace_back();
      tiles.back().reserve(tiles_across);

      for (size_t c=0; c<tiles_across; c++) {
        //all tiles start as empty
        tiles.back().push_back(tile_t((int)c,(int)r,dim,-1));
      }
    }

    //start ground height roughly 1/3 of total height
    int ground = (int) 2 * (tiles_down / 3);

    //type 0 is the fully solid ground tile
    environment::texture_constructor_t tileset_constructor;
    //set to default ground cover
    tileset_constructor.set_default_color(
      DARK_GREEN_R,
      DARK_GREEN_G,
      DARK_GREEN_B);

    //make tile 0
    tileset_constructor.set_rect(0,0,dim,dim);

    //create a random seed
    float seed = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    //walk around and generate a surface level
    for (size_t i=0; i<tiles_across; i++) {
      //create an fbm value
      float noise_val = noise::fractal_brownian_motion(seed,(float)i/tiles_across);

      //get the height at this position
      size_t height = clamp(TERRAIN_MIN_IDX,
                            ground - (int) (noise_val * 10),
                            tiles_down);

      //at the edge: add a wall
      if ((i == 0) || (i == (tiles_across - 1))) {
        tiles.at(height-1).at(i).set_type(0);
        tiles.at(height-1).at(i).set_solid(true);
        tiles.at(height-2).at(i).set_type(0);
        tiles.at(height-2).at(i).set_solid(true);
      }
      tiles.at(height).at(i).set_type(0);
      tiles.at(height).at(i).set_solid(true);
    }

    //erode the corners of the surface tiles
    //erode_grnd_corners(tileset_constructor);

    //tileset texture dimensions
    int tsw,tsh;
    //generate the tileset
    tileset = std::make_shared<tileset_t>(
      tileset_constructor.generate(renderer,tsw,tsh),
      tsw,tsh,dim
    );
  }

  /**
   * Erode the corners of ground tiles based on slope
   * @param constructor texture constructor
   */
  void procedural_tilemap_t::erode_grnd_corners(environment::texture_constructor_t& tileset_constructor) {
    //get the height of the first tile
    tile_t& prev_tile = get_grnd_tile(0);
    int prev_y_depth = prev_tile.get_y_depth();
    int curr_y_depth;
    int last_type_assignment = 0;
    grnd_tile_type prev_slope = FLAT;

    //for each column
    for (size_t i=1; i<(tiles.at(0).size() - 1); i++) {
      //get the current ground tile
      tile_t& curr_tile = get_grnd_tile(i);
      curr_y_depth = curr_tile.get_y_depth();

      //check if this tile should slope up
      if (curr_y_depth < prev_y_depth) {
        //create a new tile
        last_type_assignment++;

        prev_slope = SLOPE_L;
        //make a new tile
        mk_ground_tile(tileset_constructor,
                       last_type_assignment,
                       SLOPE_L);
        //update the type
        curr_tile.set_type(last_type_assignment);

      } else if (curr_y_depth > prev_y_depth) {
        //previous tile should slope down or both
        if (prev_slope == SLOPE_L) {
          //switch to slope in both directions
          mk_ground_tile(tileset_constructor,
                         prev_tile.get_type(),
                         SLOPE_BOTH);

          //no change to current tile
          prev_slope = FLAT;

        } else {
          //create a new tile
          last_type_assignment++;
          //make a new tile
          mk_ground_tile(tileset_constructor,
                         last_type_assignment,
                         SLOPE_L);
          //update the type
          prev_tile.set_type(last_type_assignment);

          //no change to current tile
          prev_slope = FLAT;
        }

      } else {
        //no change to current tile
        prev_slope = FLAT;
      }
    }
  }

  /**
   * Check that a position is in bounds
   * @return   whether the position is in bounds
   */
  bool procedural_tilemap_t::in_bounds(int x, int y) const {
    int idx_x = x / dim;
    int idx_y = y / dim;

    //check bounds and get the type of the tile
    return (idx_x >= 0) && (idx_y >= 0) &&
           (idx_y < (int)this->tiles.size()) &&
           (idx_x < (int)this->tiles.at(idx_y).size());
  }

  /**
   * Make a new tile
   * @param constructor texture constructor
   * @param type        the type of the tile (tileset idx)
   * @param slp         the slope of the tile
   */
  void procedural_tilemap_t::mk_ground_tile(environment::texture_constructor_t& constructor,
                                            int type,
                                            grnd_tile_type slp) {

    //offset into the map
    int offset_x = type % MAX_TILESET_WIDTH;
    int offset_y = type / MAX_TILESET_WIDTH;

    //set a rectangle
    constructor.set_rect(offset_x, offset_y, dim, dim);

    if (slp == SLOPE_L) {

    } else if (slp == SLOPE_R) {

    } else if (slp == SLOPE_BOTH) {

    }
  }

  /**
   * Get a tile (PRECOND: in_bounds called)
   * @return   the tile at that position
   */
  const tile_t& procedural_tilemap_t::get_tile(int x, int y) const {
    return this->tiles.at(y / dim).at(x / dim);
  }

  /**
   * Get the ground tile for a given column
   * @param  x col
   * @return   tile
   */
  tile_t& procedural_tilemap_t::get_grnd_tile(int x) {
    //look through each row
    for (size_t i=0; i<this->tiles.size(); i++) {
      //check if the x coord is in the bounds of this row
      if ((x >= 0) && (x < (int)tiles.at(i).size())) {
        //return first solid in col
        if (tiles.at(i).at(x).is_solid()) {
          return tiles.at(i).at(x);
        }
      }
    }
    return tiles.back().at(x);
  }

  /**
   * Get the ground tile for a given column
   * @param  x col
   * @return   tile
   */
  const tile_t& procedural_tilemap_t::get_grnd_tile(int x) const {
    //look through each row
    for (size_t i=0; i<this->tiles.size(); i++) {
      //check if the x coord is in the bounds of this row
      if ((x >= 0) && (x < (int)tiles.at(i).size())) {
        //return first solid in col
        if (tiles.at(i).at(x).is_solid()) {
          return tiles.at(i).at(x);
        }
      }
    }
    return tiles.back().at(x);
  }

  /**
   * Get the ground height at some x coordinate
   * @param  x the x coordinate
   * @return   the ground height (y)
   */
  int procedural_tilemap_t::ground_height(int x) const {
    //check bounds
    if ((x >= 0) && (x <= (width_p / dim))) {
      return get_grnd_tile(x).get_y_depth();
    }
    return 0;
  }

  /**
   * Check if the tile at a position is solid
   * @param  x the x coordinate
   * @param  y the y coordinate
   * @return   whether the tile at this position is solid
   */
  bool procedural_tilemap_t::is_solid(int x, int y) const {
    //check if position is in bounds and solid
    return this->in_bounds(x,y) &&
           this->get_tile(x,y).is_solid();
  }

  /**
   * Check if the tile at a position is liquid
   * @param  x the x coordinate
   * @param  y the y coordinate
   * @return   whether the tile at this position is liquid
   */
  bool procedural_tilemap_t::is_liquid(int x, int y) const {
    //check if position is in bounds and solid
    return this->in_bounds(x,y) &&
           this->get_tile(x,y).is_liquid();
  }

  /**
   * Check if a bounding box collides with some solid tile
   * @param  other the bounding box
   * @return       whether the bounding box collides
   */
  bool procedural_tilemap_t::is_collided(const SDL_Rect& other) const {
    //get the tiles that might intersect
    for (int i=(other.x - dim); i<(other.w + other.x + (2 * dim)); i+=(dim / 2)) {
      for (int j=(other.y - dim); j<(other.h + other.y + (2 * dim)); j+=(dim / 2)) {
        if (this->in_bounds(i,j)) {
          //get the tile
          const tile_t& curr = this->get_tile(i,j);

          //check for a solid collision
          if (curr.is_collided(other) && curr.is_solid()) {
            return true;
          }
        }
      }
    }
    return false;
  }

  /**
   * Check if a position collides with some solid tile
   * @param  x position x
   * @param  y position y
   * @return   whether this position collides with a solid tile
   */
  bool procedural_tilemap_t::is_collided(int x, int y) const {
    //generate positions that might intersect
    for (int i=(x - dim); i<(x + dim); i+=(dim / 2)) {
      for (int j=(y - dim); j<(y + dim); j+=(dim / 2)) {
        //check if the current tile is collided and solid
        if (this->in_bounds(i,j)) {

          //get the tile
          const tile_t& curr = this->get_tile(i,j);
          if (curr.is_collided(x,y) && curr.is_solid()) {
            return true;
          }
        }
      }
    }
    return false;
  }

  /**
   * Get the width of the map in pixels
   * @return the width of the map in pixels
   */
  int procedural_tilemap_t::get_width() const {
    return width_p;
  }

  /**
   * Get the height of the map in pixels
   * @return the height of the map in pixels
   */
  int procedural_tilemap_t::get_height() const {
    return height_p;
  }

  /**
   * Update any updatable tiles
   */
  void procedural_tilemap_t::update() {
    //TODO
  }

  /**
   * Render the background tilemap elements
   * Note: this includes the entity layer
   * @param renderer the sdl renderer
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void procedural_tilemap_t::render_bg(SDL_Renderer& renderer, const SDL_Rect& camera, bool debug) const {
    SDL_Rect bounds = {-1,-1,camera.w+2,camera.h+2};
    //draw the background color
    SDL_SetRenderDrawColor(&renderer,
                          LIGHT_GREEN_R,
                          LIGHT_GREEN_G,
                          LIGHT_GREEN_B,255);
    SDL_RenderFillRect(&renderer,&bounds);

    //draw the hills


    //draw tiles
    for (size_t r=0; r<tiles.size(); r++) {
      for (size_t c=0; c<tiles.at(r).size(); c++) {
        //render the tile
        tiles.at(r).at(c).render(
          renderer,
          camera,
          tileset,
          false,
          debug
        );
      }
    }
  }


  /**
   * Render the foreground tilemap elements
   * @param renderer the sdl renderer
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void procedural_tilemap_t::render_fg(SDL_Renderer& renderer, const SDL_Rect& camera, bool debug) const {
    //TODO
  }
}}
