/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_DEAD_TREE_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_DEAD_TREE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "renderable.h"
#include <string>
#include <vector>

namespace impl {
namespace environment {

  /**
   * A tree that can be knocked over to cross hazards
   */
  struct dead_tree_t : public renderable_t {
    //whether the tree has fallen
    bool felled;
    //when falling, the rotation frames so far
    int rot_frames;
    //the bounds of the tree once it has fallen
    SDL_Rect felled_bounds;

    //whether the tree is falling left (t), right (f)
    bool left;

    //the texture
    SDL_Texture* texture = NULL;
    //the dimensions of the texture
    int texture_w;
    int texture_h;

    //whether debug mode enabled
    bool debug;

  public:
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
    dead_tree_t(int x,int y,
                int w, int h,
                const std::string& texture_path,
                SDL_Renderer& renderer,
                bool left,
                bool debug);
    dead_tree_t(const dead_tree_t&) = delete;
    dead_tree_t& operator=(const dead_tree_t&) = delete;

    /**
     * Free texture
     */
    ~dead_tree_t();

    /**
     * Fell the dead tree
     */
    void interact();

    /**
     * Whether this tree can still be felled
     * @return whether this tree can be interacted with
     */
    bool is_interactive() const;

    /**
     * Whether the tree is solid (only when in the fallen state)
     * @return whether the tree has completely fallen
     */
    bool is_solid() const { return felled && (rot_frames > 0); }

    /**
     * Check if this element is in view of the camera
     * @param  recr the collision box
     * @return
     */
    bool is_collided(const SDL_Rect& rect) const;

    /**
     * Check if some coordinate collides with this tree
     * @param  x the x coordinate
     * @param  y the y coordinate
     * @return   whether this position collides with the foam bounds
     */
    bool is_collided(int x, int y) const;

    /**
     * Update the tree
     */
    void update();

    /**
     * Render the tree
     * @param renderer the renderer to use
     * @param camera   the camera
     */
    void render(SDL_Renderer& renderer, const SDL_Rect& camera) const;

  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_DEAD_TREE_H*/
