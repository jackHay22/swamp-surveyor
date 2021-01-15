/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "tilemap_state.h"
#include "../exceptions.h"

namespace impl {
namespace state {

  /**
   * Constructor
   * @param tilemap    the tilemap this state uses
   * @param entities   the entities in this state
   * @param player_idx the index of the player in the entity list
   */
  tilemap_state_t::tilemap_state_t(std::shared_ptr<tilemap::tilemap_t> tilemap,
                                   std::vector<std::shared_ptr<entity::entity_t>>& entities,
                                   int player_idx, SDL_Rect& camera)
    : state_t(camera),
      tilemap(tilemap),
      entities(entities) {
    //sanity check
    if (player_idx >= entities.size()) {
      throw exceptions::rsrc_exception_t("not enough entities found in list");
    }

    //dynamic cast on player entity
    player = std::dynamic_pointer_cast<entity::player_t>(entities.at(player_idx));

    //get the player from the list
    if (!player) {
      throw exceptions::rsrc_exception_t("player idx in entity list does not refer to player type");
    }
  }

  /**
   * Handle some key event
   * @param e the event
   */
  void tilemap_state_t::handle_event(const SDL_Event& e) {
    player->handle_event(e);
  }

  /**
   * Update this tile
   */
  void tilemap_state_t::update() {
    //update tilemap
    tilemap->update();

    //update players
    for (size_t i=0; i<entities.size(); i++) {
      entities.at(i)->update();

      //check for a collision
      if (tilemap->is_collided(entities.at(i)->get_bounds())) {
        entities.at(i)->step_back();
      }
    }

    //center the camera on the player
    int center_x, center_y;
    player->get_center(center_x,center_y);

    //set the x and y
    this->camera.x = center_x - (this->camera.w / 2);
    this->camera.y = center_y - (this->camera.h / 2);

    //bound the camera
    if (camera.x < 0) {
      camera.x = 0;
    }

    if (camera.y < 0) {
      camera.y = 0;
    }

    //bound the camera by total map width
    if (camera.x > (this->tilemap->get_width() - camera.w)) {
      camera.x = this->tilemap->get_width() - camera.w;
    }

    //bound the camera by total map height
    if (camera.y > (this->tilemap->get_height() - camera.h)) {
      camera.y = this->tilemap->get_height() - camera.h;
    }
  }

  /**
   * Render the current gamestate
   * @param renderer the renderer
   */
  void tilemap_state_t::render(SDL_Renderer& renderer) const {
    //render background layer
    tilemap->render_bg(renderer, this->camera);

    //render entities
    for (size_t i=0; i<entities.size(); i++) {
      entities.at(i)->render(renderer);
    }

    //render foreground layer
    tilemap->render_fg(renderer, this->camera);
  }
}}
