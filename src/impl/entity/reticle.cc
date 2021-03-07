/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "reticle.h"

namespace impl {
namespace entity {

  /**
   * Constructor
   * @param window_scale the scale of the window
   */
  reticle_t::reticle_t(int window_scale)
    : x(0), y(0), window_scale(window_scale) {}

  /**
   * Get the target of the reticle in level space
   * Note: this is the inverse operation to the camera conversion
   * @param x the x position in the level set by the call
   * @param y the y position in the level set by the call
   * @param camera the camera
   */
  void reticle_t::get_lvl_target(int& x,
                                 int& y,
                                 const SDL_Rect& camera) const {
    x = this->x + camera.x;
    y = this->y + camera.y;
  }

  /**
   * Handle an SDL event (to calculate mouse position)
   */
  void reticle_t::handle_event(const SDL_Event& e) {
    if (e.type == SDL_MOUSEMOTION) {
     int mx = 0;
     int my = 0;
     SDL_GetMouseState(&mx,&my);

     //scale the position
     x = mx / window_scale;
     y = my / window_scale;
   }
  }

  /**
   * Render the var
   * @param renderer the renderer to use
   */
  void reticle_t::render(SDL_Renderer& renderer) const {

    //set the color
    SDL_SetRenderDrawColor(&renderer,255,255,255,225);

    //draw the reticle
    SDL_RenderDrawPoint(&renderer,x,y - 1);
    SDL_RenderDrawPoint(&renderer,x - 1,y);
    SDL_RenderDrawPoint(&renderer,x,y + 1);
    SDL_RenderDrawPoint(&renderer,x + 1,y);
  }
}}
