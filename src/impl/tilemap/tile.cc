/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "tile.h"

namespace impl {
namespace tilemap {

    /**
     * Constructor for the tile
     * (Solid by default), can be set later
     * @param x    the x position in the map
     * @param y    the y position in the map
     * @param dim  the dimension of the tile
     * @param type the type of the tile (idx into tileset)
     */
    tile_t::tile_t(int x, int y, int dim, int type)
      : type(type), solid(true) {
      bounds.x = x;
      bounds.y = y;
      bounds.w = dim;
      bounds.h = dim;
    }

    /**
     * Check if a bounding box collides with this tile
     * @param  other the other bounding box
     * @return       whether the two boxes collide
     */
    bool tile_t::is_collided(const SDL_Rect& other) const {
      //check if the given bounding box collides with this tile
      return ((bounds.x < (other.x + other.w)) &&
              ((bounds.x + bounds.w) > other.x) &&
              (bounds.y < (other.y + other.h)) &&
              ((bounds.y + bounds.h) > other.y));
    }

    /**
     * Render this tile
     * - Checks if the camera box collides with the tile position
     * - Renders the tile texture
     * @param renderer the sdl renderer
     * @param camera the camera
     * @param tileset the tileset to sample
     */
    void tile_t::render(SDL_Renderer& renderer,
                        const SDL_Rect& camera,
                        const std::shared_ptr<tileset_t> tileset) const {
      //check the collision
      if (this->is_collided(camera)) {
        //render this tile
        tileset->render(renderer,
                        bounds.x - camera.x,
                        bounds.y - camera.y,
                        this->type);
      }
    }
}}
