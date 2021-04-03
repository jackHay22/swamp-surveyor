/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_PROCEDURAL_TILEMAP_H
#define _IO_JACKHAY_SWAMP_PROCEDURAL_TILEMAP_H

#include <vector>
#include <string>
#include <memory>
#include "tileset.h"
#include "abstract_tilemap.h"
#include "tile.h"
#include "tileset.h"
#include "static_hill_bg.h"
#include "tileset_constructor.h"
#include "map_components.h"
#include "tile_builder.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

namespace impl {
namespace tilemap {

  /**
   * Defines a procedurally generated tilemap
   * (Uses the same interface as the regular tilemap)
   */
  struct procedural_tilemap_t : public abstract_tilemap_t {
  private:
    //dimension of tiles
    int dim;

    //dimensions of map in pixels
    int width_p;
    int height_p;

    //the generated tileset
    std::shared_ptr<tileset_t> tileset;

    //ground tiles
    std::vector<std::vector<tile_t>> tiles;

    //foreground tiles
    std::vector<std::vector<tile_t>> fg_tiles;

    //hills
    std::vector<std::unique_ptr<static_hill_bg_t>> hills;

    //near ground map components
    std::unique_ptr<map_components_t> near_ground;

    //foreground map components
    std::unique_ptr<map_components_t> fore_ground;

    /**
     * Called by the constructor _after_ generating ground layer
     * @param renderer the renderer
     */
    void generate_bg(SDL_Renderer& renderer);

    /**
     * Called by the constructor to generate procedural terrain
     * @param renderer the renderer for generating textures
     */
    void generate_terrain(SDL_Renderer& renderer);

    /**
     * Erode the corners of ground tiles based on slope
     * @param constructor texture constructor
     */
    void erode_grnd_corners(tileset_constructor_t& tileset_constructor);

    /**
     * Generate the foreground
     * Created after the ground layer is generated
     * @param tileset_constructor the tileset constructor
     * @param renderer            the sdl renderer
     */
    void generate_fg(tileset_constructor_t& tileset_constructor,
                     SDL_Renderer& renderer);

    /**
     * Check that a position is in bounds
     * @return   whether the position is in bounds
     */
    bool in_bounds(int x, int y) const;

    /**
     * Get a tile (PRECOND: in_bounds called)
     * @return   the tile at that index
     */
    const tile_t& get_tile(int x, int y) const;

    /**
     * Get the ground tile for a given column
     * @param  x col
     * @return   tile
     */
    tile_t& get_grnd_tile(int x);

    /**
     * Get the ground tile for a given column
     * @param  x col
     * @return   tile
     */
    const tile_t& get_grnd_tile(int x) const;

  public:
    /**
     * Constructor
     * @param dim      tile dimension
     * @param width_p  map width in pixels
     * @param height_p map height in pixels
     */
    procedural_tilemap_t(int dim, int width_p, int height_p, SDL_Renderer& renderer);
    procedural_tilemap_t(const procedural_tilemap_t&) = delete;
    procedural_tilemap_t& operator=(const procedural_tilemap_t&) = delete;

    /**
     * Get the ground height at some x coordinate
     * @param  x the x coordinate
     * @return   the ground height (y)
     */
    int ground_height(int x) const;

    /**
     * Check if the tile at a position is solid
     * @param  x the x coordinate
     * @param  y the y coordinate
     * @return   whether the tile at this position is solid
     */
    bool is_solid(int x, int y) const override;

    /**
     * Check if the tile at a position is liquid
     * @param  x the x coordinate
     * @param  y the y coordinate
     * @return   whether the tile at this position is liquid
     */
    bool is_liquid(int x, int y) const override;

    /**
     * Check if a bounding box collides with some solid tile
     * @param  other the bounding box
     * @return       whether the bounding box collides
     */
    bool is_collided(const SDL_Rect& other) const override;

    /**
     * Check if a position collides with some solid tile
     * @param  x position x
     * @param  y position y
     * @return   whether this position collides with a solid tile
     */
    bool is_collided(int x, int y) const override;

    /**
     * Get the width of the map in pixels
     * @return the width of the map in pixels
     */
    int get_width() const override;

    /**
     * Get the height of the map in pixels
     * @return the height of the map in pixels
     */
    int get_height() const override;

    /**
     * Update any updatable tiles
     */
    void update() override;

    /**
     * Render the background tilemap elements
     * Note: this includes the entity layer
     * @param renderer the sdl renderer
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    void render_bg(SDL_Renderer& renderer, const SDL_Rect& camera, bool debug) const override;


    /**
     * Render the foreground tilemap elements
     * @param renderer the sdl renderer
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    void render_fg(SDL_Renderer& renderer, const SDL_Rect& camera, bool debug) const override;

  };
}}

#endif /*_IO_JACKHAY_SWAMP_PROCEDURAL_TILEMAP_H*/
