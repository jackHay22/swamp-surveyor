/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "tile.h"
#include <iostream>

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
    tile_t::tile_t(int x, int y, int dim, int type,bool debug)
      : type(type),
        solid(false),
        liquid(false),
        x(x), y(y),
        dim(dim),
        debug(debug) {}

    /**
     * Check if a bounding box collides with this tile
     * @param  other the other bounding box
     * @return       whether the two boxes collide
     */
    bool tile_t::is_collided(const SDL_Rect& other) const {
      int actual_x = x * dim;
      int actual_y = y * dim;
      //check if the given bounding box collides with this tile
      return ((actual_x < (other.x + other.w)) &&
              ((actual_x + dim) > other.x) &&
              (actual_y < (other.y + other.h)) &&
              ((actual_y + dim) > other.y));
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
                        (x * dim) - camera.x,
                        (y * dim) - camera.y,
                        this->type);

        if (debug) {
          SDL_Rect image_bounds = {(x * dim) - camera.x,
                                   (y * dim) - camera.y,
                                   this->dim,this->dim};

          if (solid) {
            //set the draw color
            SDL_SetRenderDrawColor(&renderer,255,0,0,127);

            //render the bounds
            SDL_RenderDrawRect(&renderer,&image_bounds);
          } else if (liquid) {
            //set the draw color
            SDL_SetRenderDrawColor(&renderer,0,0,255,127);

            //render the bounds
            SDL_RenderDrawRect(&renderer,&image_bounds);
          }
        }
      }
    }
}}
