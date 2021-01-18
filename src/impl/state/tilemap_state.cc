/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "tilemap_state.h"
#include "../exceptions.h"
#include <iostream>

namespace impl {
namespace state {

  /**
   * Constructor
   * @param tilemap    the tilemap this state uses
   * @param entities   the entities in this state
   * @param insects    the insects in the map
   * @param env_renderable environmental elements that can be rendered
   * @param level_items the items in this level
   * @param player_idx the index of the player in the entity list
   * @param manager    the state manager
   * @param camera     the level camera
   */
  tilemap_state_t::tilemap_state_t(std::shared_ptr<tilemap::tilemap_t> tilemap,
                                   std::vector<std::shared_ptr<entity::entity_t>>& entities,
                                   std::shared_ptr<entity::insects_t> insects,
                                   std::vector<std::shared_ptr<environment::renderable_t>>& env_renderable,
                                   std::vector<std::shared_ptr<items::item_t>>& level_items,
                                   int player_idx, state_manager_t& manager,
                                   SDL_Rect& camera)
    : state_t(manager, camera),
      tilemap(tilemap),
      entities(entities),
      insects(insects),
      env_renderable(env_renderable),
      level_items(level_items),
      show_bars(false),
      player_health_bar(5,120,50,1000,255,0,0) {
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
   * Check if a given bounding box is on solid ground
   * Checks at 3 collision points
   * @param  bounds the bounding box
   * @return        whether the box is against a solid component
   */
  bool tilemap_state_t::on_solid_ground(const SDL_Rect& bounds) const {
    //check the tilemap
    if (tilemap->is_solid(bounds.x - 1,
                          bounds.y + bounds.h + 1) ||
        tilemap->is_solid(bounds.x + bounds.w + 1,
                          bounds.y + bounds.h + 1) ||
        tilemap->is_solid(bounds.x + (bounds.w / 2),
                          bounds.y + bounds.h + 1)) {
      return true;
    }

    //check if the entity intersects with a renderable env component
    for (size_t i=0; i<env_renderable.size(); i++) {
      if (env_renderable.at(i)->is_solid()) {
        if (env_renderable.at(i)->is_collided(bounds.x - 1,
                                              bounds.y + bounds.h + 1) ||
            env_renderable.at(i)->is_collided(bounds.x + bounds.w + 1,
                                              bounds.y + bounds.h + 1) ||
            env_renderable.at(i)->is_collided(bounds.x + (bounds.w / 2),
                                              bounds.y + bounds.h + 1)) {
          return true;
        }
      }
    }

    //no intersections
    return false;
  }

  /**
   * Check if an entity has collided with a solid environmental object
   * @param  bounds the bounds of the entity
   * @return        whether the entity collided with the env object
   */
  bool tilemap_state_t::is_collided_solid_env(const SDL_Rect& bounds) const {
    //check each environmental element for a collision
    for (size_t i=0; i<env_renderable.size(); i++) {
      if (env_renderable.at(i)->is_solid() &&
          env_renderable.at(i)->is_collided(bounds)) {
        return true;
      }
    }
    return false;
  }

  /**
   * Handle some key event
   * @param e the event
   */
  void tilemap_state_t::handle_event(const SDL_Event& e) {
    //check for interaction keys
    player->handle_event(e);

    //the action
    environment::player_action action = environment::NONE;

    //check if key is pressed or released
    if (e.type == SDL_KEYDOWN) {
      //check key pressed
      switch (e.key.keysym.sym) {
        case SDLK_p:
          action = environment::PUSH;
          break;
        case SDLK_i:
          show_bars = !show_bars;
          break;
      }
    }

    SDL_Rect player_bounds = player->get_bounds();
    int px = player_bounds.x + (player_bounds.w / 2);
    int py = player_bounds.y + (player_bounds.h / 2);

    //perform any interactions
    for (size_t i=0; i<env_renderable.size(); i++) {
      if (env_renderable.at(i)->is_collided(player_bounds) &&
          env_renderable.at(i)->is_interactive()) {
        //interact with the environmental element
        env_renderable.at(i)->interact(action,px,py);
      }
    }
  }

  /**
   * Update this tile
   */
  void tilemap_state_t::update() {
    //update tilemap
    tilemap->update();

    //update entities
    for (size_t i=0; i<entities.size(); i++) {
      //update each entity in the y direction
      entities.at(i)->update_y();

      //check for a collision with tilemap or env elements
      if (tilemap->is_collided(entities.at(i)->get_bounds()) ||
          this->is_collided_solid_env(entities.at(i)->get_bounds())) {
        entities.at(i)->step_back_y();
      }

      //check if the entity is on solid ground
      if (this->on_solid_ground(entities.at(i)->get_bounds())) {
        //update the entity in the x direction
        entities.at(i)->update_x(*tilemap);

        //check for a collision with tilemap or env elements
        if (tilemap->is_collided(entities.at(i)->get_bounds()) ||
            this->is_collided_solid_env(entities.at(i)->get_bounds())) {
          entities.at(i)->step_back_x(*tilemap);
        }
      }

      //update entity
      entities.at(i)->update(*tilemap,env_renderable);
    }

    //get the updated player bounds
    SDL_Rect player_bounds = player->get_bounds();
    int px = player_bounds.x + (player_bounds.w / 2);
    int py = player_bounds.y + (player_bounds.h / 2);

    //update the items
    for (size_t i=0; i<level_items.size(); i++) {
      //check if the player can pick up the item
      if (level_items.at(i)->is_collided(player_bounds)) {
        //pick up this item to the center of the player
        level_items.at(i)->pick_up(px,py);
      }

      level_items.at(i)->update(*tilemap);
    }

    //update the insects
    insects->update();

    //update each renderable environment element
    for (size_t i=0; i<env_renderable.size(); i++) {
      //check for player/environment interaction
      if (env_renderable.at(i)->is_collided(player_bounds)) {
        player->do_damage(env_renderable.at(i)->get_damage());
      }

      //update the environment element
      env_renderable.at(i)->update();
    }

    //set the player health bar level
    player_health_bar.set_val(player->get_health());

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
      entities.at(i)->render(renderer,this->camera);
    }

    //render insect swarm
    insects->render(renderer,this->camera);

    //render environmental elements
    for (size_t i=0; i<env_renderable.size(); i++) {
      env_renderable.at(i)->render(renderer,this->camera);
    }

    //render items
    for (size_t i=0; i<level_items.size(); i++) {
      level_items.at(i)->render(renderer,this->camera);
    }

    //render foreground layer
    tilemap->render_fg(renderer, this->camera);

    //render indicator bars on screen
    if (show_bars) {
      player_health_bar.render(renderer);
    }
  }
}}
