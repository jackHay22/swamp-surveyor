/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TILEMAP_STATIC_HILL_BG_H
#define _IO_JACKHAY_SWAMP_TILEMAP_STATIC_HILL_BG_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

namespace impl {
namespace tilemap {

  /*
   * Procedurally generated hill textures for background
   */
  struct static_hill_bg_t {
  private:

    //the dimensions of the texture
    int width;
    int height;
    //height offset during render
    int offset;

    //the hill texture
    SDL_Texture* texture;

    /**
     * Generate textures
     */
    void generate(SDL_Renderer& renderer,
                  float fbm_persist,
                  int amplitude,
                  int r, int g, int b);
  public:
    /**
     * Constructor
     * @param renderer    renderer for creating textures
     * @param r,g,b       color
     * @param fbm_persist fractal brownian motion persistence
     * @param chunk_width the width of each chunk
     */
    static_hill_bg_t(SDL_Renderer& renderer,
                     int r,int g,int b,
                     float fbm_persist,
                     int width,
                     int height,
                     int amplitude,
                     int offset);
    static_hill_bg_t(const static_hill_bg_t&) = delete;
    static_hill_bg_t& operator=(const static_hill_bg_t&) = delete;

    //free textures
    ~static_hill_bg_t();

    /**
     * Render the hills
     * @param renderer sdl renderer
     * @param camera   camera position
     */
    void render(SDL_Renderer& renderer, const SDL_Rect& camera) const;
  };

}}

#endif /*_IO_JACKHAY_SWAMP_TILEMAP_STATIC_HILL_BG_H*/
