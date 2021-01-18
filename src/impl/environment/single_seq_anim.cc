/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "single_seq_anim.h"
#include "../utils.h"

namespace impl {
namespace environment {

  /**
   * Constructor
   * @param path     the path to the texture
   * @param renderer the renderer for loading the texture
   * @param total_frames   the total number of frames
   */
  single_seq_anim_t::single_seq_anim_t(const std::string& path,
                                       SDL_Renderer& renderer,
                                       int total_frames)
    : current_frame(0),
      total_frames(total_frames),
      flipped(false) {

    //load the texture
    texture = utils::load_texture(path,
                                  renderer,
                                  texture_width,
                                  texture_height);
    frame_width = texture_width / total_frames;
  }

  /**
   * Free the texture
   */
  single_seq_anim_t::~single_seq_anim_t() {
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }

  /**
   * Update the animation
   */
  void single_seq_anim_t::update() {
    if (current_frame < (total_frames - 1)) {
      current_frame++;
    }
  }

  /**
   * Render the animation
   * @param renderer the renderer to use
   * @param x the position x
   * @param y the position y
   */
  void single_seq_anim_t::render(SDL_Renderer& renderer, int x, int y) {
    //render the animation
    SDL_Rect sample_bounds;
    //determine the coords of the tile within the set
    sample_bounds.x = current_frame * frame_width;
    sample_bounds.y = 0;
    sample_bounds.w = frame_width;
    sample_bounds.h = texture_height;

    //the x y position to render at
    SDL_Rect image_bounds = {x,y,frame_width,texture_height};

    if (flipped) {
      image_bounds = {x - frame_width, y, frame_width, texture_height};

      //render the texture and flip
      SDL_RenderCopyEx(&renderer,
                       texture,
                       &sample_bounds,
                       &image_bounds,
                       0, NULL,
                       SDL_FLIP_HORIZONTAL);

    } else {
      //render the texture
      SDL_RenderCopy(&renderer,
                     texture,
                     &sample_bounds,
                     &image_bounds);
    }
  }
}}
