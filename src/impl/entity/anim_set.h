/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ANIM_SET_H
#define _IO_JACKHAY_SWAMP_ANIM_SET_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <string>

namespace impl {
namespace entity {

  /**
   * Animation set
   */
  struct anim_set_t {
  private:
    //number of animation frames
    int anim_frames = 8;
    //size of anim frames
    int frame_width = 0;
    int frame_height = 0;
    //timing
    int duration = 1;
    int current_frame = 0;
    //ticks since last update
    int ticks = 0;

    bool is_slow = false;

    //the texture
    SDL_Texture* texture = NULL;

    /**
     * Load the texture
     * @param path the path to the texture
     * @param renderer the renderer for loading the texture
     */
    void load(const std::string& path,
              SDL_Renderer& renderer);

  public:
    anim_set_t(const std::string& path,
               SDL_Renderer& renderer);
    anim_set_t(const anim_set_t&) = delete;
    anim_set_t& operator=(const anim_set_t&) = delete;

    /**
     * Reset the animation to the first frame
     */
    void reset() { current_frame = 0; }

    /**
     * Set the animation slower
     * @param is_slow the value
     */
    void set_slow(bool is_slow);

    /**
     * Free texture
     */
    ~anim_set_t();

    /**
     * Update the animation
     */
    void update();

    /**
     * Render the animation frame
     * @param renderer the renderer to use
     * @param x        the x position
     * @param y        the y position
     */
    void render(SDL_Renderer& renderer, int x, int y) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ANIM_SET_H*/
