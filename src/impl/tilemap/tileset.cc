/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "tileset.h"
#include "../exceptions.h"
#include "../utils.h"
#include <iostream>

namespace impl {
namespace tilemap {

  /**
   * Constructor loads tiles, throws exception on failure
   * @param rsrc_path the path to the resource
   * @param tile_dim  the dimensions of tiles
   * @param renderer the renderer to use for loading the texture
   */
  tileset_t::tileset_t(const std::string& rsrc_path,
                       int tile_dim,
                       SDL_Renderer& renderer)
    : tile_dim(tile_dim) {
    //load resource
    load(rsrc_path, renderer);
  }

  /**
   * Construct from a preloaded texture (assumes ownership)
   * @param texture the preloaded texture
   * @param w the width of the texture
   * @param h the height of the texture
   * @param tile_dim the dimension of the tiles
   */
  tileset_t::tileset_t(SDL_Texture* texture,
                       int w, int h, int tile_dim)
    : tile_dim(tile_dim),
      tiles_wide(w / tile_dim),
      tiles_high(h / tile_dim),
      texture(texture) {}

  /**
   * Destructor frees texture allocation
   */
  tileset_t::~tileset_t() {
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }

  /**
   * Load the tileset
   * Throws rsrc_exception_t
   * @param rsrc_path the path to the image file
   * @param renderer sdl renderer for loading texture
   */
  void tileset_t::load(const std::string& rsrc_path, SDL_Renderer& renderer) {
    int width, height = 0;

    //load the texture
    this->texture = utils::load_texture(rsrc_path,renderer,width,height);

    //Get image dimensions
    this->tiles_wide = width / this->tile_dim;
    this->tiles_high = height / this->tile_dim;
  }

  /**
   * Render a tile. Takes the position of the tile
   * and the type of the tile
   * @param renderer the sdl renderer
   * @param x    the x position of the tile (w/ respect to camera)
   * @param y    the y position of the tile (w/ respect to camera)
   * @param type the index of the tile in the tileset
   */
  void tileset_t::render(SDL_Renderer& renderer, int x, int y, int type) const {

    if (type >= 0) {
      SDL_Rect sample_bounds;
      //determine the coords of the tile within the set
      sample_bounds.x = (type % this->tiles_wide) * this->tile_dim;
      sample_bounds.y = (type / this->tiles_wide) * this->tile_dim;
      sample_bounds.w = this->tile_dim;
      sample_bounds.h = this->tile_dim;

      //bound the image
      SDL_Rect image_bounds = {x,y,this->tile_dim,this->tile_dim};

      //render the texture
      SDL_RenderCopy(&renderer,
                     this->texture,
                     &sample_bounds,
                     &image_bounds);
    }
  }

}}
