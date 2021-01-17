/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "foam_spray.h"
#include <stdlib.h>
#include "../../environment/chemical_foam.h"

namespace impl {
namespace entity {
namespace actions {

  #define GRAVITY_PER_TICK 1
  #define MAX_PARTICLE_VEL 4
  #define MIN_PARTICLE_VEL 2
  #define PARTICLE_COLOR_R 255
  #define PARTICLE_COLOR_G 252
  #define PARTICLE_COLOR_B 187

  /**
   * Constructor
   */
  foam_spray_t::foam_spray_t()
    : particles() {}

  /**
   * Update the action
   * @param map the tilemap
   * @param env_elements environmental elements that this action
   *  can interact with
   *  @param x  the position where spray should emit from x
   *  @param y  the position where spray should emit from y
   *  @param dir the direction (either 1 or -1)
   */
  void foam_spray_t::update(const tilemap::tilemap_t& map,
                            std::vector<std::shared_ptr<environment::renderable_t>>& env_elements,
                            int x, int y, int dir) {
    //if inactive and invisible, no update
    if (!active && !visible) {
      return;
    }

    if (active) {
      //generate a new particle
      particles.push_back(std::make_pair(x,y+1));
    }

    //update any existing particles
    for (size_t i=0; i<particles.size(); i++) {
      //update the x position in the given direction
      particles.at(i).first += (((rand() % MAX_PARTICLE_VEL) + MIN_PARTICLE_VEL) * dir);
      particles.at(i).second += GRAVITY_PER_TICK;

      int curr_x = particles.at(i).first;
      int curr_y = particles.at(i).second;

      //whether this particle will be removed from a ground/liquid collision
      bool remove = map.is_collided(curr_x,curr_y) ||
                    map.is_liquid(curr_x,curr_y);

      //check for environment element collision
      for (size_t j=0; j<env_elements.size(); j++) {
        //dynamic cast to checmical foam
        std::shared_ptr<environment::chemical_foam_t> foam =
          std::dynamic_pointer_cast<environment::chemical_foam_t>(env_elements.at(j));

        //if the particles collide, disperse some of the foam
        if (foam && foam->is_collided(curr_x,curr_y)) {
          foam->disperse_foam();
          //remove the current particle
          remove = true;
        }
      }

      if (remove) {
        //remove the particle
        particles.erase(particles.begin() + i);
      }
    }

    //check if this action is visible
    this->visible = !particles.empty();
  }

  /**
   * Render the entity
   * @param renderer the renderer to use
   * @param camera the camera
   */
  void foam_spray_t::render(SDL_Renderer& renderer,
                            const SDL_Rect& camera) const {
    //(if active we assume in view since locked to player)
    if (active || visible) {
      //temp foam color
      SDL_SetRenderDrawColor(&renderer,
                             PARTICLE_COLOR_R,
                             PARTICLE_COLOR_G,
                             PARTICLE_COLOR_B,225);
      //render foam
      for (size_t i=0; i<particles.size(); i++) {
        SDL_RenderDrawPoint(&renderer,
                            particles.at(i).first - camera.x,
                            particles.at(i).second - camera.y);
      }
    }
  }

}}}
