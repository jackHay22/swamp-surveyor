/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ABSTRACT_TILEMAP_H
#define _IO_JACKHAY_SWAMP_ABSTRACT_TILEMAP_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

namespace impl {
namespace tilemap {

  /**
   * Defines the abstract tilemap interface
   */
  struct abstract_tilemap_t {
    /**
     * Check if the tile at a position is solid
     * @param  x the x coordinate
     * @param  y the y coordinate
     * @return   whether the tile at this position is solid
     */
    virtual bool is_solid(int x, int y) const = 0;

    /**
     * Check if the tile at a position is liquid
     * @param  x the x coordinate
     * @param  y the y coordinate
     * @return   whether the tile at this position is liquid
     */
    virtual bool is_liquid(int x, int y) const = 0;

    /**
     * Check if a bounding box collides with some solid tile
     * @param  other the bounding box
     * @return       whether the bounding box collides
     */
    virtual bool is_collided(const SDL_Rect& other) const = 0;

    /**
     * Check if a position collides with some solid tile
     * @param  x position x
     * @param  y position y
     * @return   whether this position collides with a solid tile
     */
    virtual bool is_collided(int x, int y) const = 0;

    /**
     * Get the width of the map in pixels
     * @return the width of the map in pixels
     */
    virtual int get_width() const = 0;

    /**
     * Get the height of the map in pixels
     * @return the height of the map in pixels
     */
    virtual int get_height() const = 0;

    /**
     * Update any updatable tiles
     */
    virtual void update() = 0;

    /**
     * Render the background tilemap elements
     * Note: this includes the entity layer
     * @param renderer the sdl renderer
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    virtual void render_bg(SDL_Renderer& renderer, const SDL_Rect& camera, bool debug) const = 0;


    /**
     * Render the foreground tilemap elements
     * @param renderer the sdl renderer
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    virtual void render_fg(SDL_Renderer& renderer, const SDL_Rect& camera, bool debug) const = 0;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ABSTRACT_TILEMAP_H*/
