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
   */
  layer_t::layer_t(const std::string& rsrc_path,
                   int dim,
                   std::shared_ptr<tilemap::tileset_t> tileset,
                   bool debug)
    : tileset(tileset), dim(dim), debug(debug) {

    try {
      //read from the file
      std::ifstream layer_file(rsrc_path);
      int y = 0;

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

             //add the tile to the map contents
             this->contents.back().push_back(std::make_shared<tile_t>(x,y,dim,type,debug));

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
   * Check if the tile at a position is solid
   * @param  x the x coordinate
   * @param  y the y coordinate
   * @return   whether the tile at this position is solid
   */
  bool layer_t::is_solid(int x, int y) const {
    int idx_x = x / dim;
    int idx_y = y / dim;

    //check bounds and get the type of the tile
    if ((idx_y < this->contents.size()) &&
        (idx_x < this->contents.at(idx_y).size())) {
      return this->contents.at(idx_y).at(idx_x)->is_solid();

    } else {
      return -1;
    }
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
    bool collided = false;

    //check each tile
    this->for_each_const([&collided,other](tile_t& tile){
      //look for the first solid collided tile
      if (!collided && tile.is_collided(other)) {
        collided = tile.is_solid();
      }
    });

    return collided;
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
   */
  void layer_t::render(SDL_Renderer& renderer,
                       const SDL_Rect& camera) const {
    //render each tile
    this->for_each_const([&renderer,&camera,this](tile_t& t){
      //render the tile
      t.render(renderer,camera,this->tileset);
    });
  }
}}
