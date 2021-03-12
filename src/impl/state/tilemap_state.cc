/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "tilemap_state.h"
#include "../exceptions.h"
#include "../utils.h"
#include <iostream>

namespace impl {
namespace state {

  /**
   * Constructor
   * @param tilemap    the tilemap this state uses
   * @param entities   the entities in this state
   * @param insects    the insects in the map
   * @param env        environmental elements in the state
   * @param level_items the items in this level
   * @param trans_blocks transparent blocks in the level
   * @param forks      forks in the map
   * @param player_idx the index of the player in the entity list
   * @param manager    the state manager
   * @param camera     the level camera
   */
  tilemap_state_t::tilemap_state_t(std::shared_ptr<tilemap::tilemap_t> tilemap,
                                   std::vector<std::shared_ptr<entity::entity_t>>& entities,
                                   std::shared_ptr<entity::insects_t> insects,
                                   std::shared_ptr<environment::environment_t> env,
                                   std::vector<std::shared_ptr<items::item_t>>& level_items,
                                   std::vector<std::shared_ptr<tilemap::transparent_block_t>>& trans_blocks,
                                   std::vector<std::shared_ptr<misc::map_fork_t>>& forks,
                                   int player_idx, state_manager_t& manager,
                                   SDL_Rect& camera)
    : state_t(manager, camera),
      tilemap(tilemap),
      entities(entities),
      insects(insects),
      env(env),
      level_items(level_items),
      trans_blocks(trans_blocks),
      forks(forks),
      player_idx(player_idx),
      show_bars(false),
      player_health_bar(5,120,50,1000,255,0,0),
      reticle(std::make_unique<entity::reticle_t>(manager.get_window_scale()))  {
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
   * Set the player
   * @param player the player to add to state
   */
  void tilemap_state_t::set_player(std::shared_ptr<entity::player_t> player) {
    this->player = player;

    //update the player in the entities list
    entities.at(player_idx) = player;
  }

  /**
   * Check if a given bounding box is on solid ground
   * Checks at 3 collision points
   * @param  bounds the bounding box
   * @return        whether the box is against a solid component
   */
  bool tilemap_state_t::on_solid_ground(const SDL_Rect& bounds) const {
    //check the tilemap and environment
    return tilemap->is_solid(bounds.x - 1, bounds.y + bounds.h + 1) ||
           tilemap->is_solid(bounds.x + bounds.w + 1, bounds.y + bounds.h + 1) ||
           tilemap->is_solid(bounds.x + (bounds.w / 2), bounds.y + bounds.h + 1) ||
           env->is_solid(bounds.x - 1, bounds.y + bounds.h + 1) ||
           env->is_solid(bounds.x + bounds.w + 1, bounds.y + bounds.h + 1) ||
           env->is_solid(bounds.x + (bounds.w / 2), bounds.y + bounds.h + 1);
  }

  /**
   * Handle some key event
   * @param e the event
   */
  void tilemap_state_t::handle_event(const SDL_Event& e) {
    //check for interaction keys
    player->handle_event(e);

    //update the reticle (if mouse move)
    reticle->handle_event(e);

    if (e.type == SDL_MOUSEMOTION) {
      int center_x, _center_y;
      player->get_center(center_x,_center_y);

      //change player focus based on reticle move
      player->change_focus(reticle->get_position().first < (center_x - camera.x));
    }

    //the action
    environment::player_action action = environment::NONE;
    bool use_fork = false;

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
        case SDLK_f:
          use_fork = true;
          break;
      }
    }

    if (use_fork) {
      //update map forks
      for (size_t i=0; i<forks.size(); i++) {
        if (forks.at(i)->can_interact()) {
          //set the player position
          int target_x;
          int target_y;
          forks.at(i)->get_target(target_x, target_y);
          //set the player position
          player->set_position(target_x, target_y);

          //change the current state
          manager.set_state(forks.at(i)->get_dest());
        }
      }
    }

    //interact with the environment
    env->interact(action,
                  player->get_bounds(),
                  player->is_facing_left());
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
          env->is_collided(entities.at(i)->get_bounds())) {
        entities.at(i)->step_back_y();
      }

      //check if the entity is on solid ground
      if (this->on_solid_ground(entities.at(i)->get_bounds())) {
        //update the entity in the x direction
        entities.at(i)->update_x(*tilemap);

        //check for a collision with tilemap or env elements
        if (tilemap->is_collided(entities.at(i)->get_bounds()) ||
            env->is_collided(entities.at(i)->get_bounds())) {
          //TODO allow entity to climb env elements
          entities.at(i)->step_back_x(*tilemap, *env);
        }
      }

      //update entity
      entities.at(i)->update(*tilemap,*env);
    }

    //get the updated player bounds
    SDL_Rect player_bounds = player->get_bounds();
    int px = player_bounds.x + (player_bounds.w / 2);
    int py = player_bounds.y + (player_bounds.h / 2);

    //update transparent blocks
    for (size_t i=0; i<trans_blocks.size(); i++) {
      trans_blocks.at(i)->update(player_bounds);
    }

    //update map forks
    for (size_t i=0; i<forks.size(); i++) {
      forks.at(i)->update(player_bounds);
    }

    //update the items
    for (size_t i=0; i<level_items.size(); i++) {
      //check if the player can pick up the item
      if (level_items.at(i)->is_collided(player_bounds)) {
        //pick up this item to the center of the player
        level_items.at(i)->pick_up(px,py);
        //add this item to the player's inventory
        player->add_item(level_items.at(i));
      }

      level_items.at(i)->update(*tilemap);

      //if the item can be removed from the environment, do so
      //at this point the item is likely held by the player
      if (level_items.at(i)->removable()) {
        level_items.erase(level_items.begin() + i);
      }
    }

    //update the insects
    insects->update();

    //update the environment and apply any damage
    player->do_damage(env->update(player_bounds));

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
   * @param debug    whether debug mode enabled
   */
  void tilemap_state_t::render(SDL_Renderer& renderer, bool debug) const {
    //render background layer
    tilemap->render_bg(renderer, this->camera,debug);

    //render entities
    for (size_t i=0; i<entities.size(); i++) {
      entities.at(i)->render(renderer,this->camera,debug);
    }

    //render insect swarm
    insects->render(renderer,this->camera,debug);

    //render the environment
    env->render(renderer,this->camera,debug);

    //render items
    for (size_t i=0; i<level_items.size(); i++) {
      level_items.at(i)->render(renderer,this->camera,debug);
    }

    //render foreground layer
    tilemap->render_fg(renderer, this->camera,debug);

    //render transparent blocks
    for (size_t i=0; i<trans_blocks.size(); i++) {
      trans_blocks.at(i)->render(renderer,this->camera,debug);
    }

    //render map forks
    for (size_t i=0; i<forks.size(); i++) {
      forks.at(i)->render(renderer, this->camera,debug);
    }

    //render indicator bars on screen
    if (show_bars) {
      player_health_bar.render(renderer);
    }

    //render the mouse position
    reticle->render(renderer);
  }

  /**
   * Render any debug info
   * @param renderer sdl renderer
   * @param font     loaded ttf font
   */
  void tilemap_state_t::render_debug_info(SDL_Renderer& renderer, TTF_Font& font) const {
    int center_x, center_y;
    player->get_center(center_x,center_y);

    //the player position
    const std::string player_position = std::to_string(center_x) + "," + std::to_string(center_y);

    //render text
    utils::render_text(renderer,player_position,0,12,font);

    //the reticle position
    std::pair<int,int> pos = reticle->get_position();

    const std::string reticle_position = std::to_string(pos.first + camera.x) + "," +
                                         std::to_string(pos.second + camera.y);

    //render the reticle position
    utils::render_text(renderer,reticle_position,0,24,font);
  }
}}
