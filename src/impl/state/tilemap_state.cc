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
