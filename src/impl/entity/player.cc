/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "player.h"
#include <iostream>
#include "actions/foam_spray.h"
#include "actions/push.h"
#include <iostream>

namespace impl {
namespace entity {

  #define MAX_PUSH_TICKS 6

  /**
   * Empty behavior handler for player
   */
  void behavior_noop(const std::vector<entity_pos_t>&,
                     const tilemap::tilemap_t&,int,int,
                     entity_state&, bool&) {  }

  /**
   * Constructor
   * @param x                  entity position x (center)
   * @param y                  entity position y (center)
   * @param w                  entity bounds width
   * @param h                  entity bounds height
   * @param texture_paths      paths for each of the 4 basic animations
   * @param renderer           the renderer for loading textures
   * @param tile_dim           the dimension of tiles
   * @param base_path          the resource directory base path
   */
  player_t::player_t(int x, int y,
                     int w, int h,
                     const std::vector<std::string>& anim_cfg_paths,
                     SDL_Renderer& renderer,
                     int tile_dim,
                     const std::string& base_path)
    : entity_t(x,y,w,h,anim_cfg_paths,renderer,tile_dim,base_path,behavior_noop,NON_NPC),
      performing_action(false),
      actions(),
      held_items(),
      move_tick_counter(0) {
    //add actions that correspond to animations
    actions.push_back(std::make_unique<actions::foam_spray_t>());
    actions.push_back(std::make_unique<actions::push_t>());

    //load the additional animation paths
    if (anim_cfg_paths.size() > ENTITY_STATES) {
      for (size_t i=ENTITY_STATES; i<anim_cfg_paths.size(); i++) {
        //load additional animations
        anims.push_back(std::make_unique<anim_set_t>(anim_cfg_paths.at(i),
                                                     renderer,
                                                     base_path));
      }
    }
  }

  /**
   * Change the player focus if standing based on reticle
   * @param left whether the reticle is to the left of the player (right if false)
   */
  void player_t::change_focus(bool left) {
    if (state == IDLE) {
      facing_left = left;
    }
  }

  /**
   * Add a new item to inventory
   * @param item the item to add
   */
  void player_t::add_item(std::shared_ptr<items::item_t> item) {
    held_items.push_back(item);
  }

  /**
   * Handle some key event
   * @param e the event
   */
  void player_t::handle_event(const SDL_Event& e) {
    //check if key is pressed or released
    if (e.type == SDL_KEYDOWN) {
      //check key pressed
      switch (e.key.keysym.sym) {
        //case SDLK_w: state = ; break;
        case SDLK_a:
          if (state != CLIMB) {
            state = MOVE;
            facing_left = true;
          }
          break;
        //case SDLK_s: state = ; break;
        case SDLK_d:
          if (state != CLIMB) {
            state = MOVE;
            facing_left = false;
          }
          break;
        case SDLK_e:
          state = ACTION;
          performing_action = true;
          action = DISPERSE_FOAM;
          break;
        case SDLK_p:
          //no overlap
          if ((performing_action == false) || (action != PUSH)) {
            //store the current state to revert after push complete
            prev_state = state;
            state = ACTION;
            performing_action = true;
            action = PUSH;

            //reset the push animation
            anims.at(action)->reset();
            move_tick_counter = MAX_PUSH_TICKS;
          }
          break;
      }
    } else if (e.type == SDL_KEYUP) {
      //check key released
      switch (e.key.keysym.sym) {
        //case SDLK_w: state = ; break;
        case SDLK_a: state = IDLE; break;
        //case SDLK_s: state = ; break;
        case SDLK_d: state = IDLE; break;
        case SDLK_e:
          if (state == ACTION) {
            state = IDLE;
            performing_action = false;
          }
          break;
      }
    }
  }


  /**
   * Update the entity (after directional updates)
   * @param map the tilemap
   * @param env environmental elements that can be interacted
   */
  void player_t::update(const tilemap::tilemap_t& map,
                        environment::environment_t& env) {
    //call entity update
    entity_t::update(map,env);

    //if there is a limit on the action
    if ((state == ACTION) && (move_tick_counter > 0)) {
      move_tick_counter--;

      if (move_tick_counter == 0) {
        //set state to idle
        state = prev_state;
      }
    }

    //check whether the player is performing an action
    //and toggle the current action
    performing_action = state == ACTION;
    actions.at(action)->toggle_action(performing_action);

    //update the animation that corresponds to the action
    anims.at(action)->update();

    //update all actions
    SDL_Rect b = get_bounds();

    //set the origin and direction of the action
    int emit_x = b.x + b.w;
    int emit_y = b.y + (b.h / 2);
    int dir = 1;
    if (facing_left) {
      emit_x -= b.w;
      dir = -1;
    }

    //update all actions
    for (size_t i=0; i<actions.size(); i++) {
      //perform the action
      actions.at(i)->update(map,env,emit_x,emit_y,dir);
    }
  }

  /**
   * Render the entity
   * @param renderer the renderer to use
   * @param camera the camera
   * @param debug    whether debug mode enabled
   */
  void player_t::render(SDL_Renderer& renderer,
                        const SDL_Rect& camera,
                        bool debug) const {
    //entity render
    entity_t::render(renderer,camera,debug);

    //render all actions
    for (size_t i=0; i<actions.size(); i++) {
      //render the action
      actions.at(i)->render(renderer,camera,debug);
    }

    //if the player is performing an action use a supplementary animation
    if (performing_action) {
      SDL_Rect b = get_bounds();
      //actions correspond to pairs of animations
      anims.at(action)->render(renderer,
                              (b.x + (b.w / 2)) - camera.x,
                              (b.y + (b.h / 2)) - camera.y,
                              facing_left);

    }
  }
}}
