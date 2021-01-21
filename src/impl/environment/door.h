/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_DOOR_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_DOOR_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "renderable.h"
#include "single_seq_anim.h"
#include <string>
#include <memory>

namespace impl {
namespace environment {

  /**
   * A door that the player can push open
   */
  struct door_t : public renderable_t {
    //whether the door is open
    bool opened;

    //the single sequence animation
    std::unique_ptr<single_seq_anim_t> anim;

    //whether the door opened left
    bool left;

    //whether debug mode enabled
    bool debug;

  public:
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
    door_t(int x,int y,
           int w, int h,
           const std::string& texture_path,
           SDL_Renderer& renderer,
           int total_frames,
           bool debug);
    door_t(const door_t&) = delete;
    door_t& operator=(const door_t&) = delete;

    /**
     * Open the door
     * @param a the interaction type
     * @param x the player position x
     * @param y the player position y
     */
    void interact(player_action a, int x, int y);

    /**
     * Whether this door can still be opened
     * @return whether this door can be opened
     */
    bool is_interactive() const { return !opened; }

    /**
     * Whether the door is solid (only when in the closed state)
     * @return whether the door is closed
     */
    bool is_solid() const { return !opened; }

    /**
     * Check if this door collides with some bounding box
     * @param  recr the collision box
     * @param  interaction whether the collision is for interaction (t)
     * of just physics (f)
     * @return
     */
    bool is_collided(const SDL_Rect& rect,
                     bool interaction) const;

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

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_DOOR_H*/
