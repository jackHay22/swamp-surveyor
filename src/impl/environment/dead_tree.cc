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
   * @param x     position x
   * @param y     position y
   * @param w     width
   * @param h     height
   * @param texture_path the path to the tree texture
   * @param renderer the renderer for loading the texture
   * @param left  the direction the tree will fall
   * @param debug whether debug mode enabled
   */
  dead_tree_t::dead_tree_t(int x,int y,
                           int w, int h,
                           const std::string& texture_path,
                           SDL_Renderer& renderer,
                           bool left,
                           bool debug)
    : renderable_t({x,y,w,h}, true, false),
      felled(false),
      rot_frames(0),
      felled_bounds({x,y,w,h}),
      left(left),
      debug(debug) {

    //adjust the felled bounds
    if (left) {
      felled_bounds = {x + h - w, y + h, h, w};
    } else {
      felled_bounds = {x + w, y + h, h, w};
    }

    //load the texture
    this->texture = utils::load_texture(texture_path,
                                        renderer,
                                        texture_w,
                                        texture_h);
  }

  /**
   * Free texture
   */
  dead_tree_t::~dead_tree_t() {
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }

  /**
   * Fell the dead tree
   */
  void dead_tree_t::interact() {
    felled = true;
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
   * @return
   */
  bool dead_tree_t::is_collided(const SDL_Rect& rect) const {
    if (felled && (rot_frames > 0)) {
      //tree has fallen, check collision with new bounding region
      return ((felled_bounds.x < (rect.x + rect.w)) &&
              ((felled_bounds.x + felled_bounds.w) > rect.x) &&
              (felled_bounds.y < (rect.y + rect.h)) &&
              ((felled_bounds.y + felled_bounds.h) > rect.y));
    } else {
      return renderable_t::is_collided(rect);
    }
  }

  /**
   * Check if some coordinate collides with this tree
   * @param  x the x coordinate
   * @param  y the y coordinate
   * @return   whether this position collides with the foam bounds
   */
  bool dead_tree_t::is_collided(int x, int y) const {
    if (felled && (rot_frames > 0)) {
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
    //tree felled but still rotating
    if (felled && (rot_frames < ROTATIONAL_STEPS)) {
      //update rotational position
      rot_frames++;
    }
  }

  /**
   * Render the tree
   * @param renderer the renderer to use
   * @param camera   the camera
   */
  void dead_tree_t::render(SDL_Renderer& renderer, const SDL_Rect& camera) const {
    int adjusted_x = (bounds.x + (bounds.w / 2)) - (texture_w / 2);

    //draw the tree with rotation
    SDL_Rect image_bounds = {adjusted_x - camera.x,
                             bounds.y - camera.y,
                             texture_w, texture_h};

    //render the texture
    SDL_RenderCopyEx(&renderer,
                     texture,
                     NULL,
                     &image_bounds,
                     (TOTAL_ROT_DEGREES / ROTATIONAL_STEPS) * rot_frames,
                     NULL,
                     SDL_FLIP_NONE);

    if (debug) {
      //the bounds for drawing the debug box
      SDL_Rect debug_bounds = {bounds.x - camera.x,
                               bounds.y - camera.y,
                               bounds.w, bounds.h};

      if (felled) {
        //draw felled bounds
        debug_bounds = {felled_bounds.x - camera.x,
                        felled_bounds.y - camera.y,
                        felled_bounds.w, felled_bounds.h};
      }

      //set the draw color
      SDL_SetRenderDrawColor(&renderer,255,102,0,255);

      //render the bounds
      SDL_RenderDrawRect(&renderer,&debug_bounds);
    }
  }

}}
