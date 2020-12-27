/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TILE_H
#define _IO_JACKHAY_SWAMP_TILE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <memory>
#include "tileset.h"

namespace impl {
namespace tilemap {

  /**
   * Defines a tile in the map
   */
  struct tile_t {
  private:
    //the index into the tileset
    int type;

    //whether this tile is solid
    bool solid;

    //this tile's position, dimensions in the map
    SDL_Rect bounds;

  public:
    //constructor
    tile_t(int x, int y, int dim, int type);
    tile_t(const tile_t&) = delete;
    tile_t& operator=(const tile_t&) = delete;

    /**
     * Check if a bounding box collides with this tile
     * @param  other the other bounding box
     * @return       whether the two boxes collide
     */
    bool is_collided(const SDL_Rect& other) const;

    /**
     * Check whether this tile is solid
     * @return whether this tile is solid
     */
    bool is_solid() const { return this->solid; }

    /**
     * Set this tile solid
     * @param solid the setting for the tile
     */
    void set_solid(bool solid) { this->solid = solid; }

    /**
     * Render this tile
     * - Checks if the camera box collides with the tile position
     * - Renders the tile texture
     * @param renderer the sdl renderer
     * @param camera the camera
     * @param tileset the tileset to sample
     */
    void render(SDL_Renderer& renderer,
                const SDL_Rect& camera,
                const std::shared_ptr<tileset_t> tileset) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_TILE_H*/
