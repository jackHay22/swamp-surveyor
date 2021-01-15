/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TILEMAP_H
#define _IO_JACKHAY_SWAMP_TILEMAP_H

#include <vector>
#include <string>
#include "layer.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

namespace impl {
namespace tilemap {

  /**
   * Defines a loaded tilemap
   */
  struct tilemap_t {
  private:
    //the layers behind the entity layer
    std::vector<std::unique_ptr<tilemap::layer_t>> bg_layers;

    //the layer the entity interacts with
    std::unique_ptr<tilemap::layer_t> entity_layer;

    //the layers in front of the entity
    std::vector<std::unique_ptr<tilemap::layer_t>> fg_layers;

    //the indices of tiles that are solid ground
    std::vector<int> entity_layer_solid;
    //the indices of tiles that are water
    std::vector<int> entity_layer_water;

    //the dimensions of the map in pixels for camera
    int width_p;
    int height_p;

    //if debug mode enabled
    bool debug;

  public:
    tilemap_t(const std::vector<std::string>& rsrc_paths,
              std::shared_ptr<tilemap::tileset_t> tileset,
              int entity_layer,
              int dim,
              const std::vector<int>& entity_layer_solid,
              const std::vector<int>& entity_layer_water,
              bool debug);
    tilemap_t(const tilemap_t&) = delete;
    tilemap_t& operator=(const tilemap_t&) = delete;

    /**
     * Check if a bounding box collides with some solid tile
     * @param  other the bounding box
     * @return       whether the bounding box collides
     */
    bool is_collided(const SDL_Rect& other) const;

    /**
     * Get the width of the map in pixels
     * @return the width of the map in pixels
     */
    int get_width() const;

    /**
     * Get the height of the map in pixels
     * @return the height of the map in pixels
     */
    int get_height() const;

    /**
     * Update any updatable tiles
     */
    void update();

    /**
     * Render the background tilemap elements
     * Note: this includes the entity layer
     * @param renderer the sdl renderer
     * @param camera   the camera
     */
    void render_bg(SDL_Renderer& renderer, const SDL_Rect& camera) const;


    /**
     * Render the foreground tilemap elements
     * @param renderer the sdl renderer
     * @param camera   the camera
     */
    void render_fg(SDL_Renderer& renderer, const SDL_Rect& camera) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_TILEMAP_H*/
