/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "tilemap.h"
#include "../exceptions.h"

namespace impl {
namespace tilemap {

  /**
   * Constructor
   * @param rsrc_paths         the path to each layer
   * @param tileset            the tileset to use with this map
   * @param entity_layer_idx   the index of the entity layer
   * @param dim                the dimension of a tile
   * @param entity_layer_solid the indices of solid tiles
   * @param entity_layer_water the indices of liquid tiles
   * @param bg_stationary      whether the background layer is stationary
   */
  tilemap_t::tilemap_t(const std::vector<std::string>& rsrc_paths,
                       std::shared_ptr<tilemap::tileset_t> tileset,
                       int entity_layer_idx,
                       int dim,
                       const std::vector<int>& entity_layer_solid,
                       const std::vector<int>& entity_layer_water,
                       bool bg_stationary,
                       bool debug)
    : bg_layers(), fg_layers(),
      entity_layer_solid(entity_layer_solid),
      entity_layer_water(entity_layer_water),
      debug(debug) {

    //sanity check
    if ((entity_layer_idx >= rsrc_paths.size()) || (entity_layer_idx < 0)) {
      throw exceptions::rsrc_exception_t("tilemap entity layer index out of bounds");
    }

    //load the tilemap layers
    for(size_t i=0; i<rsrc_paths.size(); i++) {
      if (i == entity_layer_idx) {
        this->entity_layer =
          std::make_unique<tilemap::layer_t>(rsrc_paths.at(entity_layer_idx),
                                             dim,
                                             tileset,
                                             false,
                                             debug);

        //set solid/liquid tiles for this layer
        this->entity_layer->set_natured_tiles(entity_layer_solid,
                                              entity_layer_water);

      } else if (i < entity_layer_idx) {
        //load as a background layer (first layer marked as stationary if configured)
        this->bg_layers.push_back(std::make_unique<tilemap::layer_t>(rsrc_paths.at(i),
                                                                     dim,
                                                                     tileset,
                                                                     bg_stationary && (i == 0),
                                                                     debug));
      } else {
        //load as a foreground layer
        this->fg_layers.push_back(std::make_unique<tilemap::layer_t>(rsrc_paths.at(i),
                                                                     dim,
                                                                     tileset,
                                                                     false,
                                                                     debug));
      }
    }

    //set map pixel dimensions
    this->width_p = entity_layer->get_layer_cols() * dim;
    this->height_p = (entity_layer->get_layer_rows() - 1) * dim;
  }

  /**
   * Check if the tile at a position is solid
   * @param  x the x coordinate
   * @param  y the y coordinate
   * @return   whether the tile at this position is solid
   */
  bool tilemap_t::is_solid(int x, int y) const {
    return entity_layer->is_solid(x,y);
  }

  /**
   * Check if the tile at a position is liquid
   * @param  x the x coordinate
   * @param  y the y coordinate
   * @return   whether the tile at this position is liquid
   */
  bool tilemap_t::is_liquid(int x, int y) const {
    return entity_layer->is_liquid(x,y);
  }


  /**
   * Check if a bounding box collides with some solid tile
   * @param  other the bounding box
   * @return       whether the bounding box collides
   */
  bool tilemap_t::is_collided(const SDL_Rect& other) const {
    return entity_layer->is_collided(other);
  }

  /**
   * Get the width of the map in pixels
   * @return the width of the map in pixels
   */
  int tilemap_t::get_width() const {
    return width_p;
  }

  /**
   * Get the height of the map in pixels
   * @return the height of the map in pixels
   */
  int tilemap_t::get_height() const {
    return height_p;
  }

  /**
   * Update any updatable tiles
   */
  void tilemap_t::update() {
    //update foreground
    for (size_t i=0; i<bg_layers.size(); i++) {
      bg_layers.at(i)->update();
    }
    //update the entity layer
    entity_layer->update();

    //update background
    for (size_t i=0; i<fg_layers.size(); i++) {
      fg_layers.at(i)->update();
    }
  }

  /**
   * Render the background tilemap elements
   * Note: this includes the entity layer
   * @param renderer the sdl renderer
   * @param camera   the camera
   */
  void tilemap_t::render_bg(SDL_Renderer& renderer,
                            const SDL_Rect& camera) const {
    //render the background
    for (size_t i=0; i<bg_layers.size(); i++) {
      bg_layers.at(i)->render(renderer,camera);
    }
    //render the entity layer
    entity_layer->render(renderer,camera);
  }


  /**
   * Render the foreground tilemap elements
   * @param renderer the sdl renderer
   * @param camera   the camera
   */
  void tilemap_t::render_fg(SDL_Renderer& renderer,
                            const SDL_Rect& camera) const {
    //render the foreground
    for (size_t i=0; i<fg_layers.size(); i++) {
      fg_layers.at(i)->render(renderer,camera);
    }
  }
}}
