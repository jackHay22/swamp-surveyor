/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "dead_tree.h"
#include "../utils.h"

namespace impl {
namespace environment {

  #define ROTATIONAL_STEPS 20
  #define TOTAL_ROT_DEGREES 90

  /**
   * Dead tree constructor
   *
   * (By default the tree is set to fall right)
   *
   * @param x     position x
   * @param y     position y
   * @param w     width
   * @param h     height
   * @param texture_path the path to the tree texture
   * @param renderer the renderer for loading the texture
   * @param total_frames the number of animation frames
   */
  dead_tree_t::dead_tree_t(int x,int y,
                           int w, int h,
                           const std::string& texture_path,
                           SDL_Renderer& renderer,
                           int total_frames)
    : renderable_t({x,y,w,h}, true, false),
      felled(false),
      felled_bounds({x, y + h - w, h, w}),
      left(false) {

    //load the animation
    anim = std::make_unique<single_seq_anim_t>(texture_path,
                                               renderer,
                                               total_frames);
  }

  /**
   * Fell the dead tree
   * @param x the player position x
   * @param y the player position y
   * @param  facing_left   whether the player is facing left
   */
  void dead_tree_t::interact(player_action a, int x, int y, bool facing_left) {
    if (a == PUSH) {
      //determine the side the player is on
      left = x > (bounds.x + (bounds.w / 2));

      //check that player orientation and direction match
      if (left == facing_left) {
        //flip the animation if left
        anim->set_flipped(left);

        if (left) {
          //adjust the bounds of where the tree will land
          felled_bounds = {bounds.x - bounds.h,
                           bounds.y + bounds.h - bounds.w,
                           bounds.h,
                           bounds.w};
        }

        felled = true;
      }
    }
  }

  /**
   * Whether this tree can still be felled
   * @return whether this tree can be interacted with
   */
  bool dead_tree_t::is_interactive() const {
    if (felled) {
      return renderable_t::is_interactive();
    }
    return true;
  }

  /**
   * Check if this element is in view of the camera
   * @param  recr the collision box
   * @param  interaction whether the test is for interaction or physics
   * @return
   */
  bool dead_tree_t::is_collided(const SDL_Rect& rect,
                                bool interaction) const {
    if (felled) {
      //tree has fallen, check collision with new bounding region
      return ((felled_bounds.x < (rect.x + rect.w)) &&
              ((felled_bounds.x + felled_bounds.w) > rect.x) &&
              (felled_bounds.y < (rect.y + rect.h)) &&
              ((felled_bounds.y + felled_bounds.h) > rect.y));
    } else {
      return renderable_t::is_collided(rect, interaction);
    }
  }

  /**
   * Check if some coordinate collides with this tree
   * @param  x the x coordinate
   * @param  y the y coordinate
   * @return   whether this position collides with the foam bounds
   */
  bool dead_tree_t::is_collided(int x, int y) const {
    if (felled) {
      //tree has fallen, check collision with new bounding region
      return (x >= felled_bounds.x) &&
             (x < (felled_bounds.x + felled_bounds.w)) &&
             (y >= felled_bounds.y) &&
             (y < (felled_bounds.y + felled_bounds.h));
    } else {
      return renderable_t::is_collided(x,y);
    }
  }

  /**
   * Update the tree
   */
  void dead_tree_t::update() {
    if (felled) {
      anim->update();
    }
  }

  /**
   * Render the tree
   * @param renderer the renderer to use
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void dead_tree_t::render(SDL_Renderer& renderer,
                           const SDL_Rect& camera,
                           bool debug) const {
    if (this->is_collided(camera,false)) {
      //render the animation
      anim->render(renderer,
                   (bounds.x - (bounds.w / 2)) - camera.x,
                   bounds.y - camera.y);

      if (debug) {
        //the bounds for drawing the debug box
        SDL_Rect debug_bounds = {bounds.x - camera.x,
                                 bounds.y - camera.y,
                                 bounds.w, bounds.h};

        if (felled) {
          //draw felled bounds
          debug_bounds = {felled_bounds.x - camera.x,
                          felled_bounds.y - camera.y,
                          felled_bounds.w,
                          felled_bounds.h};
        }

        //set the draw color
        SDL_SetRenderDrawColor(&renderer,255,102,0,255);

        //render the bounds
        SDL_RenderDrawRect(&renderer,&debug_bounds);
      }
    }
  }

}}
