/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TILESET_H
#define _IO_JACKHAY_SWAMP_TILESET_H

#include <string>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

namespace impl {
namespace tilemap {

  /**
   * Defines a loaded tileset
   */
  struct tileset_t {
  private:
    //the dimension of tiles in this set
    int tile_dim;

    //the complete dimensions of the set
    int tiles_wide;
    int tiles_high;

    //loaded tiles
    SDL_Texture* texture = NULL;

    /**
     * Load the tileset
     * Throws rsrc_exception_t
     * @param rsrc_path the path to the image file
     * @param renderer the sdl renderer for loading the texture
     */
    void load(const std::string& rsrc_path,
              SDL_Renderer& renderer);

  public:
    /**
     * Constructor loads tiles, throws exception on failure
     * @param rsrc_path the path to the resource
     * @param tile_dim  the dimensions of tiles
     * @param renderer the renderer for loading texture
     */
    tileset_t(const std::string& rsrc_path,
              int tile_dim,
              SDL_Renderer& renderer);
    tileset_t(const tileset_t&) = delete;
    tileset_t& operator=(const tileset_t&) = delete;

    //destructor
    ~tileset_t();

    /**
     * Render a tile. Takes the position of the tile
     * and the type of the tile
     * @param renderer the renderer
     * @param x    the x position of the tile (w/ respect to camera)
     * @param y    the y position of the tile (w/ respect to camera)
     * @param type the index of the tile in the tileset
     */
    void render(SDL_Renderer& renderer, int x, int y, int type) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_TILESET_H*/
