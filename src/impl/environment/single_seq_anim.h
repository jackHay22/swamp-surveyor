/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_SINGLE_SEQ_ANIM_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_SINGLE_SEQ_ANIM_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <string>

namespace impl {
namespace environment {

  /**
   * An animation that is triggered and only goes through its
   * cycle once
   */
  struct single_seq_anim_t {
  private:
    //the current frame in the animation
    int current_frame;

    //total number of frames
    int total_frames;

    //whether the animation is flipped
    bool flipped;

    //the texture
    SDL_Texture* texture = NULL;

    //dimensions of the texture
    int texture_width;
    int texture_height;
    int frame_width;

  public:
    /**
     * Constructor
     * @param path     the path to the texture
     * @param renderer the renderer for loading the texture
     * @param total_frames   the total number of frames
     */
    single_seq_anim_t(const std::string& path,
                      SDL_Renderer& renderer,
                      int total_frames);
    single_seq_anim_t(const single_seq_anim_t&) = delete;
    single_seq_anim_t& operator=(const single_seq_anim_t&) = delete;

    ~single_seq_anim_t();

    /**
     * Flip the orientation of the animation
     * @param flipped whether to flip the animation direction
     */
    void set_flipped(bool flipped) { this->flipped = flipped; }

    /**
     * Update the animation
     */
    void update();

    /**
     * Render the animation
     * @param renderer the renderer to use
     * @param x the position x
     * @param y the position y
     */
    void render(SDL_Renderer& renderer, int x, int y);
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_SINGLE_SEQ_ANIM_H*/
