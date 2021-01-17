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
     * Check if a position collides with this tile
     * @param  x position x
     * @param  y position y
     * @return   whether this position collides with this tile
     */
    bool tile_t::is_collided(int x, int y) const {
      int actual_x = this->x * dim;
      int actual_y = this->y * dim;
      //check if the position collides with the tile
      return (x >= actual_x) &&
             (x < (actual_x + dim)) &&
             (y >= actual_y) &&
             (y < (actual_y + dim));
    }

    /**
     * Render this tile
     * - Checks if the camera box collides with the tile position
     * - Renders the tile texture
     * @param renderer the sdl renderer
     * @param camera the camera
     * @param tileset the tileset to sample
     * @param stationary whether this tile is stationary in the camera or not
     */
    void tile_t::render(SDL_Renderer& renderer,
                        const SDL_Rect& camera,
                        const std::shared_ptr<tileset_t> tileset,
                        bool stationary) const {
      //check the collision
      if (this->is_collided(camera) || stationary) {
        int rel_x = (x * dim);
        int rel_y = (y * dim);

        //non stationary objects displayed relative to camera
        if (!stationary) {
          rel_x -= camera.x;
          rel_y -= camera.y;
        }

        //render this tile
        tileset->render(renderer,rel_x,rel_y,this->type);

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
