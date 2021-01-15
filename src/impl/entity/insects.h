/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_INSECTS_H
#define _IO_JACKHAY_SWAMP_INSECTS_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <vector>
#include <string>

namespace impl {
namespace entity {

  /**
   * Insect swarm across the map
   */
  struct insects_t {
  private:
    //the x and y positions of each insect
    std::vector<std::pair<int,int>> positions;

    //bounds where insects can be
    int bounds_top;
    int bounds_bot;
    std::vector<std::pair<int,int>> bounds_w;

    //the color of the insect
    int r;
    int b;
    int g;

    /**
     * Check if a given point lies within the rectangle provided
     * @param pt the point to check
     * @param camera the rectangle
     * @return whether the point is in the rectangle
     */
    bool in_camera(std::pair<int,int> pt, const SDL_Rect& camera) const;

  public:
    /**
     * Initialize map wide insect swarm
     * @param cfg_path the path to the insect configuration file
     * @param renderer the renderer for creating texture
     */
    insects_t(const std::string& cfg_path,
              SDL_Renderer& renderer);
    insects_t(const insects_t&) = delete;
    insects_t& operator=(const insects_t&) = delete;

    /**
     * Update the cloud of insects
     */
    void update();

    /**
     * Render the insects
     * @param renderer the renderer to use
     * @param camera   the camera
     */
    void render(SDL_Renderer& renderer, const SDL_Rect& camera) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_INSECTS_H*/
