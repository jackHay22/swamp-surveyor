/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "door.h"

namespace impl {
namespace environment {

  //door width: used for the new bounding box
  //once opened
  #define DEFAULT_DOOR_W 12

  /**
   * Door constructor
   * @param x     position x
   * @param y     position y
   * @param w     width
   * @param h     height
   * @param texture_path the path to the door anim texture
   * @param renderer the renderer for loading the texture
   * @param total_frames the number of animation frames
   * @param debug whether debug mode enabled
   */
  door_t::door_t(int x,int y,
                 int w, int h,
                 const std::string& texture_path,
                 SDL_Renderer& renderer,
                 int total_frames)
    : renderable_t({x,y,w,h}, true,true),
      opened(false),
      left(false) {
    //load the animation
    anim = std::make_unique<single_seq_anim_t>(texture_path,
                                               renderer,
                                               total_frames);
  }

  /**
   * Open the door
   * @param a the interaction type
   * @param x the player position x
   * @param y the player position y
   * @param facing_left   whether the player is facing left
   */
  void door_t::interact(player_action a, int x, int y, bool facing_left) {
    if (a == PUSH) {
      //determine the side the player is on and flip animation
      //if necessary
      left = x > (bounds.x + (bounds.w / 2));

      //check that the player position and direction match
      if (left == facing_left) {
        anim->set_flipped(left);

        if (left) {
          bounds.x -= DEFAULT_DOOR_W;
        }
        //set the width for camera collision
        bounds.w = DEFAULT_DOOR_W;

        //set opened
        opened = true;
      }
    }
  }

  /**
   * Check if this door collides with some bounding box
   * @param  recr the collision box
   * @param  interaction whether the collision is for interaction (t)
   * of just physics (f)
   * @return
   */
  bool door_t::is_collided(const SDL_Rect& rect,
                   bool interaction) const {
    if (interaction) {
      int actual_x = bounds.x - (DEFAULT_DOOR_W / 2);
      //test for collision with larger width
      return ((actual_x < (rect.x + rect.w)) &&
              ((actual_x + DEFAULT_DOOR_W) > rect.x) &&
              (bounds.y < (rect.y + rect.h)) &&
              ((bounds.y + bounds.h) > rect.y));
    } else {
      return renderable_t::is_collided(rect,interaction);
    }
  }

  /**
   * Update the tree
   */
  void door_t::update() {
    if (opened) {
      anim->update();
    }
  }

  /**
   * Render the tree
   * @param renderer the renderer to use
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void door_t::render(SDL_Renderer& renderer,
                      const SDL_Rect& camera,
                      bool debug) const {
    if (this->is_collided(camera,false)) {

      int texture_x = bounds.x + (DEFAULT_DOOR_W * (opened && left));
      //render animation
      anim->render(renderer,
                   texture_x - camera.x,
                   bounds.y - camera.y);

      if (debug) {
        //the bounds for drawing the debug box
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
