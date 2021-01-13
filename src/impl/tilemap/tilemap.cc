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
   * @param rsrc_paths   the path to each map file in order
   * @param tileset      the tileset that this map uses
   * @param entity_layer_idx the index of the entity layer
   */
  tilemap_t::tilemap_t(const std::vector<std::string>& rsrc_paths,
                       std::shared_ptr<tilemap::tileset_t> tileset,
                       int entity_layer_idx,
                       const std::vector<int>& entity_layer_solid,
                       const std::vector<int>& entity_layer_water)
    : bg_layers(), fg_layers(),
      entity_layer_solid(entity_layer_solid),
      entity_layer_water(entity_layer_water) {

    //sanity check
    if ((entity_layer_idx >= rsrc_paths.size()) || (entity_layer_idx < 0)) {
      throw exceptions::rsrc_exception_t("tilemap entity layer index out of bounds");
    }

    //load the tilemap layers
    for(size_t i=0; i<rsrc_paths.size(); i++) {
      if (i == entity_layer_idx) {
        this->entity_layer =
          std::make_unique<tilemap::layer_t>(rsrc_paths.at(entity_layer_idx),
                                             tileset);
      } else if (i < entity_layer_idx) {
        //load as a background layer
        this->bg_layers.push_back(std::make_unique<tilemap::layer_t>(rsrc_paths.at(i),
                                                                     tileset));
      } else {
        //load as a foreground layer
        this->fg_layers.push_back(std::make_unique<tilemap::layer_t>(rsrc_paths.at(i),
                                                                     tileset));
      }
    }
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
