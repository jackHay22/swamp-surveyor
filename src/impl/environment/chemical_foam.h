/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_CHEMICAL_FOAM_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_CHEMICAL_FOAM_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "renderable.h"
#include <string>
#include <vector>
#include <utility>
#include <tuple>

namespace impl {
namespace environment {

  /**
   * A chemical foam that will hurt the player
   */
  struct chemical_foam_t : public renderable_t {
    //any bubbles from the foam
    //these rise slowly and pop
    std::vector<std::tuple<int,int,int>> bubbles;

    //foam volume (TODO: make texture?)
    std::vector<std::pair<int,int>> foam;

    //whether debug mode enabled
    bool debug;

    /**
     * Create a random bubble
     */
    void mk_random_bubble();

  public:
    /**
     * Chemical foam constructor
     * @param x     position x
     * @param y     position y
     * @param w     width
     * @param h     height
     * @param density the density of the foam
     * @param debug whether debug mode enabled
     */
    chemical_foam_t(int x,int y,
                    int w, int h,
                    float density,
                    bool debug);
    chemical_foam_t(const chemical_foam_t&) = delete;
    chemical_foam_t& operator=(const chemical_foam_t&) = delete;

    /**
     * If the player has collided with the bounding box, do
     * any environment interaction
     * @param player the player
     */
    void player_interact(entity::player_t& player);

    /**
     * Update the renderable component
     */
    void update();

    /**
     * Render the component
     * @param renderer the renderer to use
     * @param camera   the camera
     */
    void render(SDL_Renderer& renderer, const SDL_Rect& camera) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_CHEMICAL_FOAM_H*/
