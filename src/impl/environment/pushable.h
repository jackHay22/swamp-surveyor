/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_PUSHABLE_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_PUSHABLE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "renderable.h"
#include <string>

namespace impl {
namespace environment {

  /**
   * A component that can be pushed
   * bottom is collidable surface
   */
  struct pushable_t : public renderable_t {
    //max/min position this element can be pushed to
    int min_x;
    int max_x;
    //whether the item is moving left (right if false)
    bool left;
    //whether the element is moving
    int moving_frames;

    //the y position (and above) where the element is solid
    int solid_y;

    //the texture
    SDL_Texture* texture = NULL;
    int texture_w;
    int texture_h;

    //whether debug mode enabled
    bool debug;

  public:
    /**
     * Constructor
     * @param x            starting position (x)
     * @param y            starting position (y)
     * @param w            dimension (w)
     * @param h            dimension (h)
     * @param range_x      the distance that this object can be moved in the x dir
     * @param solid_y      the y index where the element is solid
     * @param texture_path path to texture
     * @param renderer     renderer for loading texture
     * @param debug        whether debug mode enabled
     */
    pushable_t(int x,int y,
               int w, int h,
               int range_x,
               int solid_y,
               const std::string& texture_path,
               SDL_Renderer& renderer,
               bool debug);
    pushable_t(const pushable_t&) = delete;
    pushable_t& operator=(const pushable_t&) = delete;

    /**
     * Free texture
     */
    ~pushable_t();

    /**
     * Check if this element collides with some bounding box
     * @param  recr the collision box
     * @param  interaction whether the collision is for interaction (t)
     * of just physics (f)
     * @return
     */
    bool is_collided(const SDL_Rect& rect,
                     bool interaction) const;

    /**
     * Push the object
     * @param a the interaction type
     * @param x the player position x
     * @param y the player position y
     */
    void interact(player_action a, int x, int y);

    /**
     * Update the element
     */
    void update();

    /**
     * Render the element
     * @param renderer the renderer to use
     * @param camera   the camera
     */
    void render(SDL_Renderer& renderer, const SDL_Rect& camera) const;

  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_PUSHABLE_H*/
