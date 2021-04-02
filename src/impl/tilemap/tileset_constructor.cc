/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "tileset_constructor.h"

namespace impl {
namespace tilemap {

  /**
   * Constructor
   */
  tileset_constructor_t::tileset_constructor_t(int dim)
    : dim(dim),
      texture_constructor(),
      last_type(-1) {}

  /**
   * Get the x offset of a tile in the map
   * @param  tile the tile
   * @return      the x offset
   */
  int tileset_constructor_t::offset_x(int tile) {
    return (tile % TILESET_MAX_WIDTH) * dim;
  }

  /**
   * Get the y offset of a tile in the map
   * @param  tile the tile
   * @return the offset
   */
  int tileset_constructor_t::offset_y(int tile) {
    return (tile / TILESET_MAX_WIDTH) * dim;
  }

  /**
   * Set the default drawing color
   */
  void tileset_constructor_t::set_default_color(Uint8 r, Uint8 g, Uint8 b) {
    texture_constructor.set_default_color(r,g,b);
  }

  /**
   * Set a pixel
   */
  void tileset_constructor_t::set(int tile, int x, int y) {
    texture_constructor.set(
      offset_x(tile) + x,
      offset_y(tile) + y
    );
  }

  /**
   * Fill a tile with some color
   */
  void tileset_constructor_t::fill_tile(int tile, Uint8 r, Uint8 g, Uint8 b) {
    //set the default color
    texture_constructor.set_default_color(r,g,b);

    texture_constructor.set_rect(
      offset_x(tile), offset_y(tile),
      dim,dim
    );
  }

  /**
   * Draw a rectangle in some tile
   * @param tile the tile
   * @param x,y   origin
   * @param w,h   dimensions
   */
  void tileset_constructor_t::draw_rect(int tile, int x, int y, int w, int h) {
    texture_constructor.set_rect(
      offset_x(tile), offset_y(tile),
      w,h
    );
  }

  /**
   * Draw a line on some tile
   * @param tile the tile
   * @param x1,y1  p1
   * @param x2,y2 p2
   */
  void tileset_constructor_t::draw_line(int tile, int x1, int y1, int x2, int y2, int thickness) {
    int offx = offset_x(tile);
    int offy = offset_y(tile);

    //set the line with the correct offset
    texture_constructor.set_line(
      x1 + offx,
      y1 + offy,
      x2 + offx,
      y2 + offy,
      thickness
    );
  }

  /**
   * Erase a pixel in a given tile
   * @param tile the tile
   * @param x    pos x
   * @param y    pos y
   */
  void tileset_constructor_t::erase(int tile, int x, int y) {
    texture_constructor.erase(x + offset_x(tile), y + offset_y(tile));
  }

  /**
   * Add pixels to the tile
   * Note: these are relocated within the texture so each position
   * should be in the range 0 -> dim, 0 -> dim
   * Since the tiles aren't animated, frames are disregarded
   * @param pxs the pixels to add
   * @return the tile type
   */
  [[nodiscard]] int tileset_constructor_t::add_tile(const std::vector<environment::px_t>& pxs) {
    last_type++;

    for (size_t i=0; i<pxs.size(); i++) {
      texture_constructor.set(
        std::get<PINFO_POS>(pxs.at(i)).first + offset_x(last_type),
        std::get<PINFO_POS>(pxs.at(i)).second + offset_y(last_type),
        std::get<0>(std::get<PINFO_RGB>(pxs.at(i))),
        std::get<1>(std::get<PINFO_RGB>(pxs.at(i))),
        std::get<2>(std::get<PINFO_RGB>(pxs.at(i)))
      );
    }

    return last_type;
  }

  /**
   * Create space for a new tile without setting any pixels
   * @return the tile type
   */
  [[nodiscard]] int tileset_constructor_t::add_tile() {
    last_type++;
    return last_type;
  }

  /**
   * Generate the tileset so far
   * @param renderer the renderer
   * @return the tileset
   */
  std::shared_ptr<tileset_t> tileset_constructor_t::generate_tileset(SDL_Renderer& renderer) const {
    int tsw,tsh;
    SDL_Texture *t = texture_constructor.generate(renderer,tsw,tsh);
    //make tileset
    return std::make_shared<tileset_t>(t,tsw,tsh,dim);
  }

}}
