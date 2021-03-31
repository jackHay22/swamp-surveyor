/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "layer.h"
#include "../exceptions.h"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace impl {
namespace tilemap {

  #define COMMA ','

  /**
   * Constructor
   * @param rsrc_path the path to the map contents
   * @param dim       the tile dimension
   * @param tileset   the tileset that this map is associated with
   * @param stationary whether this layer is stationary
   */
  layer_t::layer_t(const std::string& rsrc_path,
                   int dim,
                   std::shared_ptr<tilemap::tileset_t> tileset,
                   bool stationary)
    : tileset(tileset), dim(dim), stationary(stationary)  {

    try {
      //read from the file
      std::ifstream layer_file(rsrc_path);
      int y = 0;

      std::shared_ptr<tile_t> blank = std::make_shared<tile_t>(-1,-1,dim,-1);

      //read each line of tiles
      std::string line;
      while (std::getline(layer_file, line)) {
        int x = 0;
        //split line by commas
        this->contents.emplace_back();
        std::stringstream s_stream(line);

        //get each value
        while(s_stream.good()) {
           std::string substr;
           std::getline(s_stream, substr, COMMA);

           //attempt to parse value
           try {
             int type = std::stoi(substr);

             if (type != -1) {
               //add the tile to the map contents
               this->contents.back().push_back(std::make_shared<tile_t>(x,y,dim,type));
             } else {
               this->contents.back().push_back(blank);
             }

           } catch (...) { }
           x++;
        }
        y++;
      }

    } catch (...) {
      //failed to load from file
      throw exceptions::rsrc_exception_t(rsrc_path);
    }
  }

  /**
   * Check that a position is in bounds
   * @return   whether the position is in bounds
   */
  bool layer_t::in_bounds(int x, int y) const {
    int idx_x = x / dim;
    int idx_y = y / dim;

    //check bounds and get the type of the tile
    return (idx_x >= 0) && (idx_y >= 0) &&
           (idx_y < (int)this->contents.size()) &&
           (idx_x < (int)this->contents.at(idx_y).size());
  }

  /**
   * Get a tile (PRECOND: in_bounds called)
   * @return   the tile at that position
   */
  const tile_t& layer_t::get_tile(int x, int y) const {
    return *this->contents.at(y / dim).at(x / dim);
  }

  /**
   * Check if the tile at a position is solid
   * @param  x the x coordinate
   * @param  y the y coordinate
   * @return   whether the tile at this position is solid
   */
  bool layer_t::is_solid(int x, int y) const {
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
  bool layer_t::is_liquid(int x, int y) const {
    //check if position is in bounds and solid
    return this->in_bounds(x,y) &&
           this->get_tile(x,y).is_liquid();
  }

  /**
   * Do some function on each tile
   */
  void layer_t::for_each(std::function<void(tile_t&)> fn) {
    for (size_t i=0; i<this->contents.size(); i++) {
      for (size_t j=0; j<this->contents.at(i).size(); j++) {
        fn(*this->contents.at(i).at(j));
      }
    }
  }

  /**
   * Do some function on each tile
   */
  void layer_t::for_each_const(std::function<void(tile_t&)> fn) const {
    for (size_t i=0; i<this->contents.size(); i++) {
      for (size_t j=0; j<this->contents.at(i).size(); j++) {
        fn(*this->contents.at(i).at(j));
      }
    }
  }

  /**
   * Get the number of tile rows loaded in the map
   * @return the number of rows in this layer
   */
  int layer_t::get_layer_rows() const {
    return this->contents.size();
  }

  /**
   * Get the number of tile cols loaded in the map
   * @return the number of cols in this layer
   */
  int layer_t::get_layer_cols() const {
    int max_row = 0;

    //get the maximum row length
    for (size_t i=0; i<this->contents.size(); i++) {
      int row_len = this->contents.at(i).size();
      if (row_len > max_row) {
        max_row = row_len;
      }
    }
    return max_row;
  }

  /**
   * Set tiles in this layer to be solid/liquid if their indices
   * are in the list provided
   * @param solid a list of tile indices that are solid
   * @param liquid a list of tile inndices that are liquid
   */
  void layer_t::set_natured_tiles(const std::vector<int>& solid,
                                  const std::vector<int>& liquid) {
    //set the nature for each tile
    this->for_each([&solid,&liquid](tile_t& tile) {

      //check if this type is registered as a solid
      if (std::find(solid.begin(), solid.end(), tile.get_type()) != solid.end()) {
        tile.set_solid(true);
      } else if (std::find(liquid.begin(), liquid.end(), tile.get_type()) != liquid.end()) {
        tile.set_liquid(true);
      }
    });
  }

  /**
   * Check if a bounding box collides into a solid tile in
   * layer
   * @param  other the bounding box
   * @return       whether the bounding box collides
   */
  bool layer_t::is_collided(const SDL_Rect& other) const {

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
  bool layer_t::is_collided(int x, int y) const {
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
   * Update any animated layers
   */
  void layer_t::update() {
    //update any updatable tiles
    for (size_t i=0; i<this->updatable.size(); i++) {
      this->updatable.at(i)->update();
    }
  }

  /**
   * Render this layer
   * @param renderer the sdl renderer
   * @param camera the current position of the camera
   * @param debug    whether debug mode enabled
   */
  void layer_t::render(SDL_Renderer& renderer,
                       const SDL_Rect& camera,
                       bool debug) const {
    //render each tile
    this->for_each_const([&renderer,&camera,this, debug](tile_t& t){
      //render the tile
      t.render(renderer,camera,this->tileset, this->stationary, debug);
    });
  }
}}
