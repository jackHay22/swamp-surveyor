/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "anim_set.h"
#include "../exceptions.h"
#include "../utils.h"

namespace impl {
namespace entity {

  /**
   * Construct the animation set
   * @param path            the resource path
   * @param renderer        the renderer for loading images
   * @param anim_frames     the number of frames in the set
   * @param ticks_per_frame the number of ticks between frame changes
   */
  anim_set_t::anim_set_t(const std::string& path,
                         SDL_Renderer& renderer,
                         int anim_frames,
                         int ticks_per_frame)
    : anim_frames(anim_frames),
      ticks_per_frame(ticks_per_frame),
      current_frame(0) {

    //load the texture
    load(path,renderer);
  }

  /**
  * Load the texture
  * @param path the path to the texture
  * @param renderer the renderer for loading the texture
  */
  void anim_set_t::load(const std::string& path,
                        SDL_Renderer& renderer) {
    int width = 0;

    //load the texture
    this->texture = utils::load_texture(path,renderer, width, this->frame_height);

    //Get animation frame width
    this->frame_width = width / this->anim_frames;
  }

  /**
  * Free texture
  */
  anim_set_t::~anim_set_t() {
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }

  /**
  * Update the animation
  */
  void anim_set_t::update() {
    current_frame++;
    current_frame %= anim_frames;
  }

  /**
  * Render the animation frame
  * @param renderer the renderer to use
  * @param x        the x position
  * @param y        the y position
  */
  void anim_set_t::render(SDL_Renderer& renderer, int x, int y) const {
    //create a clip for the current frame
    SDL_Rect sample_bounds;
    //determine the coords of the tile within the set
    sample_bounds.x = this->current_frame * this->frame_width;
    sample_bounds.y = 0;
    sample_bounds.w = this->frame_width;
    sample_bounds.h = this->frame_height;

    //the x y position to render at
    SDL_Rect image_bounds = {x, y,
                             x + this->frame_width,
                             y + this->frame_height};

    //render the current animation frame
    SDL_RenderCopyEx(&renderer,
                     this->texture,
                     &sample_bounds,
                     &image_bounds,
                     0,NULL,SDL_FLIP_NONE);
  }
}}
