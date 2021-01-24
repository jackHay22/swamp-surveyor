/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_H

#include <vector>
#include <memory>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "renderable.h"
#include <functional>

namespace impl {
namespace environment {

  /**
   * Environmental Components
   */
  struct environment_t {
  private:
    //renderable environment components
    std::vector<std::shared_ptr<environment::renderable_t>> env_renderable;

  public:
    /**
     * Constructor
     * @param env_renderable the list of environment elements
     */
    environment_t(std::vector<std::shared_ptr<environment::renderable_t>>& env_renderable);
    environment_t(const environment_t&) = delete;
    environment_t& operator=(const environment_t&) = delete;

    /**
     * Check if an element at a position is solid
     * @param  x the x coordinate
     * @param  y the y coordinate
     * @return   whether an element at this position is solid
     */
    bool is_solid(int x, int y) const;

    /**
     * Check whether the player has collided with solid elements in the env
     * @param  bounds the bounds of the player
     * @return        whether the bounds intersect with a solid env element
     */
    bool is_collided(const SDL_Rect& bounds) const;

    /**
     * Call a function on all elements of some type in the environment
     * @param of_type [description]
     */
    template <typename T>
    void for_each(std::function<void(T&)> fn) {
      for (size_t i=0; i<env_renderable.size(); i++) {
        std::shared_ptr<T> e = std::dynamic_pointer_cast<T>(env_renderable.at(i));
        if (e) {
          fn(*e);
        }
      }
    }

    /**
     * Player interaction with env elements that collide
     * @param  action        the player action
     * @param  player_bounds the bounds of the player
     */
    void interact(player_action action, const SDL_Rect& player_bounds);

    /**
     * Update elements
     * @param  player_bounds the bounds of the player
     * @return               total damage accumulated to apply to the player
     */
    [[nodiscard]] int update(const SDL_Rect& player_bounds);

    /**
     * Render environmental elements
     * @param renderer the sdl renderer
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    void render(SDL_Renderer& renderer, const SDL_Rect& camera, bool debug) const;

  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_H*/
