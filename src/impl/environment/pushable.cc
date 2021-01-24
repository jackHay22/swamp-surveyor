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
   * @param interact_bounds the interactive bounds
   * @param solid_bounds the bounds of the solid component
   * @param range_x      the distance that this object can be moved in the x dir
   * @param texture_path path to texture
   * @param renderer     renderer for loading texture
   * @param debug        whether debug mode enabled
   */
  pushable_t::pushable_t(SDL_Rect& interact_bounds,
                         SDL_Rect& solid_bounds,
                         int range_x,
                         const std::string& texture_path,
                         SDL_Renderer& renderer)
    : renderable_t(solid_bounds,true,true),
      min_x(solid_bounds.x - range_x),
      max_x(solid_bounds.x + solid_bounds.w + range_x),
      left(false),
      moving_frames(0),
      interact_bounds(interact_bounds) {
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
      //interaction collision
      return ((interact_bounds.x < (rect.x + rect.w)) &&
              ((interact_bounds.x + interact_bounds.w) > rect.x) &&
              (interact_bounds.y < (rect.y + rect.h)) &&
              ((interact_bounds.y + interact_bounds.h) > rect.y));
    }
    //physics collision
    return renderable_t::is_collided(rect,interaction);
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
      left = x > (interact_bounds.x + (interact_bounds.w / 2));
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
        interact_bounds.x--;
      } else if (!left && (bounds.x < max_x)) {
        bounds.x++;
        interact_bounds.x++;
      }
      //update the moving frames
      moving_frames--;
    }
  }

  /**
   * Render the element
   * @param renderer the renderer to use
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void pushable_t::render(SDL_Renderer& renderer,
                          const SDL_Rect& camera,
                          bool debug) const {
    //check if visible
    if (this->is_collided(camera,true)) {
      //center the texture on the interactive bounds
      int texture_x = (interact_bounds.x + (interact_bounds.w / 2)) - (texture_w / 2);
      int texture_y = (interact_bounds.y + (interact_bounds.h / 2)) - (texture_h / 2);
      //bounds
      SDL_Rect sample_bounds = {0,0,texture_w,texture_h};
      SDL_Rect image_bounds = {texture_x - camera.x,
                               texture_y - camera.y,
                               texture_w,texture_h};

      //render the texture
      SDL_RenderCopy(&renderer,
                     texture,
                     &sample_bounds,
                     &image_bounds);

      if (debug) {
        //render the solid portion
        SDL_Rect solid_bounds = {bounds.x - camera.x,
                                 bounds.y - camera.y,
                                 bounds.w, bounds.h};
        //set the draw color
        SDL_SetRenderDrawColor(&renderer,255,0,0,255);

        //render the bounds
        SDL_RenderDrawRect(&renderer,&solid_bounds);

        //render the interactive portion
        SDL_Rect interact_bounds = {interact_bounds.x - camera.x,
                                    interact_bounds.y - camera.y,
                                    interact_bounds.w, interact_bounds.h};

        //set the draw color
        SDL_SetRenderDrawColor(&renderer,255,102,0,255);

        //render the bounds
        SDL_RenderDrawRect(&renderer,&interact_bounds);
      }
    }
  }

}}
