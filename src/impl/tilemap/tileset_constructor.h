/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TILEMAP_TILESET_CONSTRUCTOR_H
#define _IO_JACKHAY_SWAMP_TILEMAP_TILESET_CONSTRUCTOR_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <memory>
#include "tileset.h"
#include "../environment/texture_constructor.h"

namespace impl {
namespace tilemap {

  //the max width of a tileset row
  #define TILESET_MAX_WIDTH 64

  /*
   * Procedurally generated background
   * Contains foliage, etc
   */
  struct tileset_constructor_t {
  private:
    //the dimension of tiles
    int dim;
    //texture constructor
    environment::texture_constructor_t texture_constructor;

    //the last type
    int last_type;

    /**
     * Get the x offset of a tile in the map
     * @param  tile the tile
     * @return      the x offset
     */
    int offset_x(int tile);

    /**
     * Get the y offset of a tile in the map
     * @param  tile the tile
     * @return the offset
     */
    int offset_y(int tile);

  public:
    /**
     * Constructor
     */
    tileset_constructor_t(int dim);
    tileset_constructor_t(const tileset_constructor_t&) = delete;
    tileset_constructor_t& operator=(const tileset_constructor_t&) = delete;

    /**
     * Set the default drawing color
     */
    void set_default_color(Uint8 r, Uint8 g, Uint8 b);

    /**
     * Draw a rectangle in some tile
     * @param tile the tile
     * @param x,y   origin
     * @param w,h   dimensions
     */
    void draw_rect(int tile, int x, int y, int w, int h);

    /**
     * Draw a line on some tile
     * @param tile the tile
     * @param x1,y1  p1
     * @param x2,y2 p2
     */
    void draw_line(int tile, int x1, int y1, int x2, int y2, int thickness=1);

    /**
     * Erase a pixel in a given tile
     * @param tile the tile
     * @param x    pos x
     * @param y    pos y
     */
    void erase(int tile, int x, int y);

    /**
     * Add pixels to the tile
     * Note: these are relocated within the texture so each position
     * should be in the range 0 -> dim, 0 -> dim
     * Since the tiles aren't animated, frames are disregarded
     * @param pxs the pixels to add
     * @return the tile type
     */
    [[nodiscard]] int add_tile(const std::vector<environment::px_t>& pxs);

    /**
     * Create space for a new tile without setting any pixels
     * @return the tile type
     */
    [[nodiscard]] int add_tile();

    /**
     * Generate the tileset so far
     * @param renderer sdl renderer for generating texture
     * @return the tileset
     */
    std::shared_ptr<tileset_t> generate_tileset(SDL_Renderer& renderer) const;
  };

}}

#endif /*_IO_JACKHAY_SWAMP_TILEMAP_TILESET_CONSTRUCTOR_H*/
