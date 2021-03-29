/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "environment.h"
#include "procedural_elem.h"

namespace impl {
namespace environment {

  /**
   * Constructor
   * @param env_renderable the list of environment elements
   */
  environment_t::environment_t(std::vector<std::shared_ptr<environment::renderable_t>>& env_renderable)
    : env_renderable(env_renderable) {}

  /**
   * Check if an element at a position is solid
   * @param  x the x coordinate
   * @param  y the y coordinate
   * @return   whether an element at this position is solid
   */
  bool environment_t::is_solid(int x, int y) const {
    for (size_t i=0; i<env_renderable.size(); i++) {
      //check if the player has collided with a solid element
      if (env_renderable.at(i)->is_solid() &&
          env_renderable.at(i)->is_collided(x,y)) {
        return true;
      }
    }
    return false;
  }

  /**
   * Check whether the player has collided with solid elements in the env
   * @param  bounds the bounds of the player
   * @return        whether the bounds intersect with a solid env element
   */
  bool environment_t::is_collided(const SDL_Rect& bounds) const {
    for (size_t i=0; i<env_renderable.size(); i++) {
      if (env_renderable.at(i)->is_collided(bounds,false) &&
          env_renderable.at(i)->is_solid()) {
        return true;
      }
    }
    return false;
  }

  /**
   * Player interaction with env elements that collide
   * @param  action        the player action
   * @param  player_bounds the bounds of the player
   * @param  facing_left   whether the player is facing left
   */
  void environment_t::interact(player_action action,
                               const SDL_Rect& player_bounds,
                               bool facing_left) {
    //compute player center
    int center_x = player_bounds.x + (player_bounds.w / 2);
    int center_y = player_bounds.y + (player_bounds.h / 2);

    for (size_t i=0; i<env_renderable.size(); i++) {
      //check if player can interact with env element
      if (env_renderable.at(i)->is_collided(player_bounds,true) &&
          env_renderable.at(i)->is_interactive()) {
        //interact with element
        env_renderable.at(i)->interact(action,center_x,center_y,facing_left);
      }
    }
  }

  /**
   * Update elements
   * @param  player_bounds the bounds of the player
   * @return               total damage accumulated to apply to the player
   */
  int environment_t::update(const SDL_Rect& player_bounds) {
    int total_damage = 0;
    //update each renderable environment element
    for (size_t i=0; i<env_renderable.size(); i++) {
      //check for player/environment interaction
      if (env_renderable.at(i)->is_collided(player_bounds,true)) {
        total_damage += env_renderable.at(i)->get_damage();
      }

      //update the environment element
      env_renderable.at(i)->update();
    }

    //return the total env damage to the player
    return total_damage;
  }

  /**
   * Render environmental elements
   * @param renderer the sdl renderer
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void environment_t::render(SDL_Renderer& renderer,
                             const SDL_Rect& camera,
                             bool debug) const {
    //render each element
    for (size_t i=0; i<env_renderable.size(); i++) {
      env_renderable.at(i)->render(renderer,camera,debug);
    }
  }

  /**
   * Render environmental elements in the background
   * @param renderer the sdl renderer
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void environment_t::render_bg(SDL_Renderer& renderer,
                                const SDL_Rect& camera,
                                bool debug) const {
    //render any elements that have background components
    for (size_t i=0; i<env_renderable.size(); i++) {
      //check if this is a procedural element (may have background components)
      std::shared_ptr<procedural_elem_t> proc
        = std::dynamic_pointer_cast<procedural_elem_t>(env_renderable.at(i));

      if (proc) {
        proc->render_bg(renderer,camera,debug);
      }
    }
  }

}}
