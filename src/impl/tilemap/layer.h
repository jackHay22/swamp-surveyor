/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_LAYER_H
#define _IO_JACKHAY_SWAMP_LAYER_H

#include <vector>
#include <memory>
#include <functional>
#include "tile.h"
#include "update_tile.h"
#include "tileset.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

namespace impl {
namespace tilemap {

  /**
   * Defines a tilemap layer
   */
  struct layer_t {
  private:
    //the actual map (indices into tileset)
    std::vector<std::vector<std::shared_ptr<tile_t>>> contents;

    //non static tiles that get updated (animations/interactive)
    std::vector<std::shared_ptr<update_tile_t>> updatable;

    //the tileset that this map uses
    std::shared_ptr<tileset_t> tileset;

    //if debug mode enabled
    bool debug;

    /**
     * Do some function on each tile
     */
    void for_each(std::function<void(tile_t&)> fn);

    /**
     * Do some function on each tile
     */
    void for_each_const(std::function<void(tile_t&)> fn) const;

  public:
    //constructor (throws exception if can't load)
    layer_t(const std::string& rsrc_path,
            int dim,
            std::shared_ptr<tilemap::tileset_t> tileset,
            bool debug);
    layer_t(const layer_t&) = delete;
    layer_t& operator=(const layer_t&) = delete;

    /**
     * Get the number of tile rows loaded in the map
     * @return the number of rows in this layer
     */
    int get_layer_rows() const;

    /**
     * Get the number of tile cols loaded in the map
     * @return the number of cols in this layer
     */
    int get_layer_cols() const;

    /**
     * Set tiles in this layer to be solid/liquid if their indices
     * are in the list provided
     * @param solid a list of tile indices that are solid
     * @param liquid a list of tile inndices that are liquid
     */
    void set_natured_tiles(const std::vector<int>& solid,
                           const std::vector<int>& liquid);

    /**
     * Check if a bounding box collides into a solid tile in
     * layer
     * @param  other the bounding box
     * @return       whether the bounding box collides
     */
    bool is_collided(const SDL_Rect& other) const;

    /**
     * Update any animated layers
     */
    void update();

    /**
     * Render this layer
     * @param renderer the sdl renderer
     * @param camera the current position of the camera
     */
    void render(SDL_Renderer& renderer, const SDL_Rect& camera) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_LAYER_H*/
