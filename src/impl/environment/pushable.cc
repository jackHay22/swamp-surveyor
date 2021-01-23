/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "pushable.h"
#include "../utils.h"
#include <iostream>

namespace impl {
namespace environment {

  #define MOVING_FRAMES_PER_PUSH 6

  /**
   * Constructor
   * @param x            starting position (x)
   * @param y            starting position (y)
   * @param w            dimension (w)
   * @param h            dimension (h)
   * @param range_x      the range that this object can be moved in the x dir
   * @param solid_y      the y index where the element is solid
   * @param texture_path path to texture
   * @param renderer     renderer for loading texture
   * @param debug        whether debug mode enabled
   */
  pushable_t::pushable_t(int x,int y,
                         int w, int h,
                         int range_x,
                         int solid_y,
                         const std::string& texture_path,
                         SDL_Renderer& renderer,
                         bool debug)
    : renderable_t({x,y,w,h},true,true),
      min_x(x - range_x),
      max_x(x + range_x),
      left(false),
      moving_frames(0),
      solid_y(solid_y),
      debug(debug) {
    //load the texture
    texture = utils::load_texture(texture_path,
                                  renderer,
                                  texture_w,
                                  texture_h);
  }

  /**
   * Free the texture
   */
  pushable_t::~pushable_t() {
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }

  /**
   * Check if this element collides with some bounding box
   * @param  recr the collision box
   * @param  interaction whether the collision is for interaction (t)
   * of just physics (f)
   * @return
   */
  bool pushable_t::is_collided(const SDL_Rect& rect,
                               bool interaction) const {
    if (interaction) {
      //bounds collision
      return renderable_t::is_collided(rect,interaction);

    } else {
      int solid_h = bounds.h - (solid_y - bounds.y);
      //calculate collision based on solid portion of element
      return ((bounds.x < (rect.x + rect.w)) &&
              ((bounds.x + bounds.w) > rect.x) &&
              (solid_y < (rect.y + rect.h)) &&
              ((solid_y + solid_h) > rect.y));
    }
  }

  /**
   * Push the object
   * @param a the interaction type
   * @param x the player position x
   * @param y the player position y
   */
  void pushable_t::interact(player_action a, int x, int y) {
    if (a == PUSH) {
      //determine the side the player is on and flip animation
      //if necessary
      left = x > (bounds.x + (bounds.w / 2));
      moving_frames = MOVING_FRAMES_PER_PUSH;
    }
  }

  /**
   * Update the element
   */
  void pushable_t::update() {
    if (moving_frames > 0) {
      if (left && (bounds.x > min_x)) {
        bounds.x--;
      } else if (!left && (bounds.x < max_x)) {
        bounds.x++;
      }
      //update the moving frames
      moving_frames--;
    }
  }

  /**
   * Render the element
   * @param renderer the renderer to use
   * @param camera   the camera
   */
  void pushable_t::render(SDL_Renderer& renderer, const SDL_Rect& camera) const {
    //check if visible
    if (this->is_collided(camera,true)) {
      //bounds
      SDL_Rect sample_bounds = {0,0,texture_w,texture_h};
      SDL_Rect image_bounds = {bounds.x - camera.x,
                               bounds.y - camera.y,
                               texture_w,texture_h};

      //render the texture
      SDL_RenderCopy(&renderer,
                     texture,
                     &sample_bounds,
                     &image_bounds);

      if (debug) {
        //render the solid portion
        SDL_Rect solid_bounds = {bounds.x - camera.x,
                                 solid_y - camera.y,
                                 bounds.w,
                                 bounds.h - (solid_y - bounds.y)};

        //std::cout << bounds.y << ", " << solid_y << "," << solid_bounds.h << std::endl;
        //set the draw color
        SDL_SetRenderDrawColor(&renderer,255,102,0,255);

        //render the bounds
        SDL_RenderDrawRect(&renderer,&solid_bounds);
      }
    }
  }

}}
