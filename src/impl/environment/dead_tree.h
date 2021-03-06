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
#include "single_seq_anim.h"
#include <string>
#include <vector>
#include <memory>

namespace impl {
namespace environment {

  /**
   * A tree that can be knocked over to cross hazards
   */
  struct dead_tree_t : public renderable_t {
    //whether the tree has fallen
    bool felled;
    //the bounds of the tree once it has fallen
    SDL_Rect felled_bounds;

    //the single sequence animation
    std::unique_ptr<single_seq_anim_t> anim;

    //whether the tree is falling left (t), right (f)
    bool left;

  public:
    /**
     * Dead tree constructor
     * @param x     position x
     * @param y     position y
     * @param w     width
     * @param h     height
     * @param texture_path the path to the tree texture
     * @param renderer the renderer for loading the texture
     * @param total_frames the number of animation frames
     */
    dead_tree_t(int x,int y,
                int w, int h,
                const std::string& texture_path,
                SDL_Renderer& renderer,
                int total_frames);
    dead_tree_t(const dead_tree_t&) = delete;
    dead_tree_t& operator=(const dead_tree_t&) = delete;

    /**
     * Fell the dead tree
     * @param a the interaction type
     * @param x the player position x
     * @param y the player position y
     * @param  facing_left   whether the player is facing left
     */
    void interact(player_action a, int x, int y, bool facing_left);

    /**
     * Whether this tree can still be felled
     * @return whether this tree can be interacted with
     */
    bool is_interactive() const;

    /**
     * Whether the tree is solid (only when in the fallen state)
     * @return whether the tree has completely fallen
     */
    bool is_solid() const { return felled; }

    /**
     * Check if this element is in view of the camera
     * @param  recr the collision box
     * @param interaction whether the test is for interaction or physics
     * @return
     */
    bool is_collided(const SDL_Rect& rect, bool interaction) const;

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
     * @param debug    whether debug mode enabled
     */
    void render(SDL_Renderer& renderer,
                const SDL_Rect& camera,
                bool debug) const;

  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_DEAD_TREE_H*/
