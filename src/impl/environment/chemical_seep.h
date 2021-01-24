/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_CHEMICAL_SEEP_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_CHEMICAL_SEEP_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "renderable.h"
#include <string>
#include <vector>
#include <utility>

namespace impl {
namespace environment {

  /**
   * A chemical seep dripping from some location
   * The player must capture a sample of the seep
   */
  struct chemical_seep_t : public renderable_t {
    //drips
    std::vector<std::pair<int,int>> drips;

  public:
    /**
     * Chemical seep constructor
     * @param x     position x
     * @param y     position y
     * @param w     width
     * @param h     height
     */
    chemical_seep_t(int x,int y, int w, int h);
    chemical_seep_t(const chemical_seep_t&) = delete;
    chemical_seep_t& operator=(const chemical_seep_t&) = delete;

    /**
     * Interact with the seep (collecting sample)
     * @param a the interaction
     * @param x the player position x
     * @param y the player position y
     */
    void interact(player_action a, int x, int y);

    /**
     * Update the seep
     */
    void update();

    /**
     * Render the seep
     * @param renderer the renderer to use
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    void render(SDL_Renderer& renderer,
                const SDL_Rect& camera,
                bool debug) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_CHEMICAL_SEEP_H*/
