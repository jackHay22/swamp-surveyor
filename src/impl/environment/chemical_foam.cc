/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "chemical_foam.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <random>

namespace impl {
namespace environment {

  //the amount of damage that foam does for each tick
  #define FOAM_PLAYER_DAMAGE 1
  #define FOAM_R 255
  #define FOAM_G 255
  #define FOAM_B 255
  #define BUBBLE_TICKS_PER_RISE 6

  /**
   * Chemical foam constructor
   * @param x     position x
   * @param y     position y
   * @param w     width
   * @param h     height
   * @param density the density of the foam
   * @param debug whether debug mode enabled
   */
  chemical_foam_t::chemical_foam_t(int x,int y,
                                   int w, int h,
                                   float density,
                                   bool debug)
    : renderable_t({x,y,w,h}),
      bubbles(), foam(), debug(debug) {

    //init random seed
    srand(time(NULL));
    std::vector<int> x_count;

    //normal distribution generator
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(w/2,w/4);

    //add a vertical count for each x coord
    for (int i=0; i<w; i++) {
      x_count.push_back(0);
    }

    //distribution
    for (int i=0; i<(w * h * density); i++) {
      double number = distribution(generator);
      if ((number > 0.0) &&
          (number < w) &&
          ((rand() % w) < (density * w))) {
        //add a point to this x point
        x_count.at((int)number) = x_count.at((int) number) + 1;
      }
    }

    //generate coordinates
    for (size_t i=0; i<x_count.size(); i++) {
      for (int j=0; (j < x_count.at(i) && j < h); j++) {
        //stack foam
        foam.push_back(std::make_pair(x + i, y + h - 1 - j));
      }
    }

    //generate bubbles (start at plane and rise slowly before popping)
    for (int i=0; i<(w / 2); i++) {
      mk_random_bubble();
    }
  }

  /**
   * Create a random bubble
   */
  void chemical_foam_t::mk_random_bubble() {
    bubbles.push_back(std::make_tuple((rand() % bounds.w) + bounds.x,
                                      bounds.y + bounds.h - 1,
                                      rand() % BUBBLE_TICKS_PER_RISE));
  }

  /**
   * If the player has collided with the bounding box, do
   * any environment interaction
   * @param player the player
   */
  void chemical_foam_t::player_interact(entity::player_t& player) {
    player.do_damage(FOAM_PLAYER_DAMAGE);
  }

  /**
   * Update the renderable component
   */
  void chemical_foam_t::update() {
    //small changes to foam
    //TODO

    //update existing bubbles
    for (size_t i=0; i<bubbles.size(); i++) {
      //decrement the movement tick
      std::get<2>(bubbles.at(i))--;

      //if the cycle has elapsed, move
      if (std::get<2>(bubbles.at(i)) <= 0) {
        std::get<2>(bubbles.at(i)) = BUBBLE_TICKS_PER_RISE;
        std::get<1>(bubbles.at(i))--;

        //check if the bubble is too high
        if ((std::get<1>(bubbles.at(i)) > bounds.h) &&
            (rand() % 3 == 0)) {
          //remove
          bubbles.erase(bubbles.begin() + i);
          //replace
          mk_random_bubble();
        }
      }
    }
  }

  /**
   * Render the component
   * @param renderer the renderer to use
   * @param camera   the camera
   */
  void chemical_foam_t::render(SDL_Renderer& renderer,
                               const SDL_Rect& camera) const {
    //check if this element is in view
    if (this->collides(camera)) {
      //temp foam color
      SDL_SetRenderDrawColor(&renderer,FOAM_R,FOAM_G,FOAM_B,225);
      //render foam
      for (size_t i=0; i<foam.size(); i++) {
        SDL_RenderDrawPoint(&renderer,
                            foam.at(i).first - camera.x,
                            foam.at(i).second - camera.y);
      }

      //render bubbles
      for (size_t i=0; i<bubbles.size(); i++) {
        SDL_RenderDrawPoint(&renderer,
                            std::get<0>(bubbles.at(i)) - camera.x,
                            std::get<1>(bubbles.at(i)) - camera.y);
      }

      if (debug) {

        //get the current bounds (corrected by camera view)
        SDL_Rect debug_bounds = {bounds.x - camera.x,
                                 bounds.y - camera.y,
                                 bounds.w, bounds.h};

        //set the draw color
        SDL_SetRenderDrawColor(&renderer,255,102,0,255);

        //render the bounds
        SDL_RenderDrawRect(&renderer,&debug_bounds);
      }
    }
  }

}}
