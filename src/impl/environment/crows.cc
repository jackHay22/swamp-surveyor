/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "crows.h"

namespace impl {
namespace environment {

  /**
   * Crows constructor
   * @param count        the number of crows in the map at all times
   * @param width        the width of the map (crows moving left start beyond this point)
   * @param anim_path    the path to the tree texture
   * @param base_path    the path to the resources folder
   * @param renderer the renderer for loading the texture
   */
  crows_t::crows_t(int count,
                   int width,
                   const std::string& anim_path,
                   const std::string& base_path,
                   SDL_Renderer& renderer)
    : renderable_t({0,0,0,0},false,false),
      crows(),
      width(width) {

    //create the animation
    anim = std::make_unique<entity::anim_set_t>(anim_path,
                                                renderer,
                                                base_path);
    //add random crow positions
    for (int i=0; i<count; i++) {
      crows.push_back(this->new_crow(true));
    }

    srand(time(NULL));

  }

  /**
   * Create a new crow
   * @return the crow position and direction
   */
  std::tuple<int,int,bool> crows_t::new_crow(bool initial) {
    bool left = ((rand() % 2) == 0);
    //random height
    float y = 15 + ((rand() % 30) - 15);

    //starting x based on direction
    float x = 0;
    if (left) {
      x = width;
    }

    if (initial) {
      //put the crow anywhere
      x = (rand() % width);
    }

    return std::make_tuple(x,y,left);
  }

  /**
   * Update the crows
   */
  void crows_t::update() {
    anim->update();

    //crows to remove (and then replace)
    std::vector<int> reset;

    for (size_t i=0; i<crows.size(); i++) {
      if (std::get<2>(crows.at(i))) {
        std::get<0>(crows.at(i))-=0.5;

        if (std::get<0>(crows.at(i)) < 0) {
          reset.push_back(i);
        }
      } else {
        std::get<0>(crows.at(i))+=0.5;

        if (std::get<0>(crows.at(i)) > width) {
          reset.push_back(i);
        }
      }
    }

    //reset any crows out of bounds
    for (size_t i=0; i<reset.size(); i++) {
      crows.at(i) = this->new_crow(false);
    }
  }

  /**
   * Check if a given point lies within the rectangle provided
   * @param crow the bounds of the crow to check
   * @param camera the rectangle
   * @return whether the point is in the rectangle
   */
  bool crows_t::in_camera(const SDL_Rect& crow, const SDL_Rect& camera) const {
    //check if this crow is in the camera
    return ((crow.x < (camera.x + camera.w)) &&
            ((crow.x + crow.w) > camera.x) &&
            (crow.y < (camera.y + camera.h)) &&
            ((crow.y + crow.h) > camera.y));
  }

  /**
   * Render the crows
   * @param renderer the renderer to use
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void crows_t::render(SDL_Renderer& renderer,
                       const SDL_Rect& camera,
                       bool debug) const {

    std::pair<int,int> crow_frame = anim->get_frame_size();

    SDL_Rect crow_bounds = {0,0,crow_frame.first,crow_frame.second};

    //render each crow at it's position
    for (size_t i=0; i<crows.size(); i++) {
      //set the current bounds
      crow_bounds.x = (int)std::get<0>(crows.at(i)) - (crow_bounds.w / 2);
      crow_bounds.y = (int)std::get<1>(crows.at(i)) - (crow_bounds.h / 2);

      //check camera intersection
      if (in_camera(crow_bounds, camera)) {
        anim->render(renderer,
                     (int)std::get<0>(crows.at(i)) - camera.x,
                     (int)std::get<1>(crows.at(i)) - camera.y,
                     std::get<2>(crows.at(i)));
      }
    }
  }
}}
