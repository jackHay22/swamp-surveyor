/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "layer.h"
#include "../exceptions.h"

namespace impl {
namespace tilemap {

  /**
   * Constructor
   * @param rsrc_path the path to the map contents
   * @param tileset   the tileset that this map is associated with
   */
  layer_t::layer_t(const std::string& rsrc_path,
                   std::shared_ptr<tilemap::tileset_t> tileset)
    : tileset(tileset) {

    //attempt to load file
    //throw exceptions::rsrc_exception_t(rsrc_path);
  }

  /**
   * Check if a bounding box collides into a solid tile in
   * layer
   * @param  other the bounding box
   * @return       whether the bounding box collides
   */
  bool layer_t::is_collided(const SDL_Rect& other) const {
    //check tiles for collision
    for (size_t i=0; i<this->contents.size(); i++) {
      for (size_t j=0; j<this->contents.at(i).size(); j++) {
        if (this->contents.at(i).at(j)->is_collided(other)) {
          return true;
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
   */
  void layer_t::render(SDL_Renderer& renderer,
                       const SDL_Rect& camera) const {
    //render all tiles in layer (only those colliding with camera)
    for (size_t i=0; i<this->contents.size(); i++) {
      for (size_t j=0; j<this->contents.at(i).size(); j++) {
        //render this tile
        this->contents.at(i).at(j)->render(renderer,camera,tileset);
      }
    }
  }
}}
