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

    //whether this tile is liquid
    bool liquid;

    //this tile's position
    int x;
    int y;
    int dim;

  public:
    //constructor
    tile_t(int x, int y, int dim, int type);
    tile_t(const tile_t&);
    tile_t& operator=(const tile_t&) = delete;

    /**
     * Get the height of this tile
     * @return height
     */
    int get_y_depth() const { return y * dim; }

    /**
     * Get the type of this tile
     * @return the type of this tile
     */
    int get_type() const { return type; }

    /**
     * Set the tyep of this tile
     * @param  type the tile type
     */
    void set_type(int type) { this->type = type; }

    /**
     * Check if a bounding box collides with this tile
     * @param  other the other bounding box
     * @return       whether the two boxes collide
     */
    bool is_collided(const SDL_Rect& other) const;

    /**
     * Check if a position collides with this tile
     * @param  x position x
     * @param  y position y
     * @return   whether this position collides with this tile
     */
    bool is_collided(int x, int y) const;

    /**
     * Check whether this tile is solid
     * @return whether this tile is solid
     */
    bool is_solid() const { return this->solid; }

    /**
     * Check whether this tile is liquid
     * @return whether this tile is liquid
     */
    bool is_liquid() const { return this->liquid; }

    /**
     * Set this tile solid
     * @param solid the setting for the tile
     */
    void set_solid(bool solid) { this->solid = solid; }

    /**
     * Set this tile liquid
     * @param liquid the setting for the tile
     */
    void set_liquid(bool liquid) { this->liquid = liquid; }

    /**
     * Render this tile
     * - Checks if the camera box collides with the tile position
     * - Renders the tile texture
     * @param renderer the sdl renderer
     * @param camera the camera
     * @param tileset the tileset to sample
     * @param stationary whether this tile is stationary in the camera or not
     * @param debug    whether debug mode enabled
     */
    void render(SDL_Renderer& renderer,
                const SDL_Rect& camera,
                const std::shared_ptr<tileset_t> tileset,
                bool stationary,
                bool debug) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_TILE_H*/
