/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_LAYER_H
#define _IO_JACKHAY_SWAMP_LAYER_H

#include <vector>
#include <memory>
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

  public:
    //constructor (throws exception if can't load)
    layer_t(const std::string& rsrc_path,
            std::shared_ptr<tilemap::tileset_t> tileset);
    layer_t(const layer_t&) = delete;
    layer_t& operator=(const layer_t&) = delete;

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
