/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "static_hill_bg.h"
#include "noise.h"
#include "../environment/texture_constructor.h"

namespace impl {
namespace tilemap {

  /**
   * Constructor
   * @param renderer    renderer for creating textures
   * @param r,g,b       color
   * @param fbm_persist fractal brownian motion persistence
   * @param chunk_width the width of each chunk
   */
  static_hill_bg_t::static_hill_bg_t(SDL_Renderer& renderer,
                   int r,int g,int b,
                   float fbm_persist,
                   int width,
                   int height,
                   int amplitude,
                   int offset)
    : width(width),
      height(height),
      texture(NULL) {

    //generate textures
    this->generate(renderer,fbm_persist,amplitude,r,g,b,offset);
  }

  /**
   * Destructor to free sdl textures
   */
  static_hill_bg_t::~static_hill_bg_t() {
    if (this->texture != NULL) {
      SDL_DestroyTexture(this->texture);
      texture=NULL;
    }
  }

  /**
   * Generate textures
   */
  void static_hill_bg_t::generate(SDL_Renderer& renderer,
                                 float fbm_persist,
                                 int amplitude,
                                 int r, int g, int b,
                                 int offset) {

    //fbm random seed
    float fbm_seed = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    float noise_val;
    int hill_height;

    environment::texture_constructor_t texture_constructor;
    texture_constructor.set_default_color(r,g,b);

    for (int i=0; i<width; i++) {
      noise_val = noise::fractal_brownian_motion(fbm_seed,
                                                 (float)i/width,
                                                 fbm_persist);

      hill_height = noise_val * amplitude;

      if (hill_height >= height) {
        hill_height = height - 4;
      }

      //render this slice
      texture_constructor.set_line(
        i,hill_height + offset,i,height + offset,1
      );
    }

    //set the texture
    texture = texture_constructor.generate(renderer,width,height);
  }

  /**
   * Render the hills
   * @param renderer sdl renderer
   * @param camera   camera position
   */
  void static_hill_bg_t::render(SDL_Renderer& renderer, const SDL_Rect& camera) const {

    if (texture != NULL) {
      //data to sample from the hills texture
      SDL_Rect sample_bounds = {camera.x, 0, camera.w, height};

      //the x y position to render at
      SDL_Rect image_bounds = {0,0,camera.w,height};

      SDL_RenderCopy(&renderer,
                     this->texture,
                     &sample_bounds,
                     &image_bounds);
    }
  }

}}
