/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "player.h"
#include <iostream>
#include "actions/foam_spray.h"
#include "actions/push.h"

namespace impl {
namespace entity {

  #define MAX_PUSH_TICKS 8

  /**
   * Constructor
   * @param x                  entity position x (center)
   * @param y                  entity position y (center)
   * @param w                  entity bounds width
   * @param h                  entity bounds height
   * @param texture_paths      paths for each of the 4 basic animations
   * @param renderer           the renderer for loading textures
   * @param tile_dim           the dimension of tiles
   */
  player_t::player_t(int x, int y,
                     int w, int h,
                     const std::vector<std::string>& anim_cfg_paths,
                     SDL_Renderer& renderer,
                     int tile_dim,
                     bool debug)
    : entity_t(x,y,w,h,anim_cfg_paths,renderer,tile_dim,debug),
      performing_action(false),
      actions(),
      move_tick_counter(0) {
    //add actions that correspond to animations
    actions.push_back(std::make_unique<actions::foam_spray_t>());
    actions.push_back(std::make_unique<actions::push_t>());

    //load the additional animation paths
    if (anim_cfg_paths.size() > 8) {
      for (size_t i=8; i<anim_cfg_paths.size(); i++) {
        //load additional animations
        anims.push_back(std::make_unique<anim_set_t>(anim_cfg_paths.at(i),
                                                     renderer));
      }
    }
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
            state = ACTION;
            performing_action = true;
            action = PUSH;

            //reset the push animation
            anims.at((2 * action) + !facing_left)->reset();
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
   * @param env_elements environmental elements that can be interacted
   */
  void player_t::update(const tilemap::tilemap_t& map,
              std::vector<std::shared_ptr<environment::renderable_t>>& env_elements) {
    //call entity update
    entity_t::update(map,env_elements);

    //if there is a limit on the action
    if ((state == ACTION) && (move_tick_counter > 0)) {
      move_tick_counter--;

      if (move_tick_counter == 0) {
        //set state to idle
        state = IDLE;
      }
    }

    //check whether the player is performing an action
    //and toggle the current action
    performing_action = state == ACTION;
    actions.at(action)->toggle_action(performing_action);

    //update the animation that corresponds to the action
    anims.at((2 * action) + !facing_left)->update();

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
      actions.at(i)->update(map,env_elements,emit_x,emit_y,dir);
    }
  }

  /**
   * Render the entity
   * @param renderer the renderer to use
   * @param camera the camera
   */
  void player_t::render(SDL_Renderer& renderer, const SDL_Rect& camera) const {
    //render all actions
    for (size_t i=0; i<actions.size(); i++) {
      //render the action
      actions.at(i)->render(renderer,camera);
    }

    //if the player is performing an action use a supplementary animation
    if (performing_action) {
      SDL_Rect b = get_bounds();
      //actions correspond to pairs of animations
      anims.at((2 * action) + !facing_left)->render(renderer,
                                                    (b.x + (b.w / 2)) - camera.x,
                                                    (b.y + (b.h / 2)) - camera.y);

    } else {
      //entity renders state
      entity_t::render(renderer,camera);
    }
  }
}}
