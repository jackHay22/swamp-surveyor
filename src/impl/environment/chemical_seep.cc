/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "chemical_seep.h"
#include <stdlib.h>

namespace impl {
namespace environment {

  //1/n chance of a drip per tick
  #define DRIP_CHANCE_PER_TICK 20
  #define GRAVITY_PER_TICK 2
  #define SPLASH_RADIUS 4
  #define DRIP_R 255
  #define DRIP_G 255
  #define DRIP_B 255

  /**
   * Chemical seep constructor
   * @param x     position x
   * @param y     position y
   * @param w     width
   * @param h     height
   * @param debug whether debug mode enabled
   */
  chemical_seep_t::chemical_seep_t(int x,int y,
                                   int w, int h)
    : renderable_t({x,y,w,h},true,false),
      drips() {}

  /**
   * Interact with the seep (collecting sample)
   * @param a the interaction
   * @param x the player position x
   * @param y the player position y
   */
  void chemical_seep_t::interact(player_action a, int x, int y) {}

  /**
   * Update the seep
   */
  void chemical_seep_t::update() {
    //update current drips
    for (size_t i=0; i<drips.size(); i++) {
      drips.at(i).second += GRAVITY_PER_TICK;

      //remove splashes
      if (drips.at(i).first != bounds.x) {
        drips.erase(drips.begin() + i);

      } else if (drips.at(i).second > (bounds.y + bounds.h)) {
        //turn drip into a splash
        drips.at(i).second -= rand() % SPLASH_RADIUS + 2;
        drips.at(i).first += (rand() % (2 * SPLASH_RADIUS)) - SPLASH_RADIUS;
      }
    }

    //random chance of creating a new drip
    if (rand() % DRIP_CHANCE_PER_TICK == 0) {
      drips.push_back(std::make_pair(bounds.x,bounds.y));
    }
  }

  /**
   * Render the seep
   * @param renderer the renderer to use
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void chemical_seep_t::render(SDL_Renderer& renderer,
                               const SDL_Rect& camera,
                               bool debug) const {

    if (this->is_collided(camera,false)) {
      //set the color
      SDL_SetRenderDrawColor(&renderer,DRIP_R,DRIP_G,DRIP_B,225);

      for (size_t i=0; i<drips.size(); i++) {
        SDL_RenderDrawPoint(&renderer,
                            drips.at(i).first - camera.x,
                            drips.at(i).second - camera.y);
      }

      if (debug) {
        //render the bounds
        SDL_SetRenderDrawColor(&renderer,53,81,92,225);
        //get the current bounds (corrected by camera view)
        SDL_Rect debug_bounds = {bounds.x - camera.x - 1,
                                 bounds.y - camera.y,
                                 bounds.w, bounds.h};
        //render the bounds
        SDL_RenderDrawRect(&renderer,&debug_bounds);
      }
    }
  }
}}
