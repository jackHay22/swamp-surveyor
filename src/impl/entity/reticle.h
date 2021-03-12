/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_RETICLE_H
#define _IO_JACKHAY_SWAMP_RETICLE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

namespace impl {
namespace entity {

  /**
   * The player's aim reticle
   */
  struct reticle_t {
  private:
    //the position in the screen
    int x;
    int y;

    //the scale of the window
    int window_scale;

  public:
    /**
     * Constructor
     * @param window_scale the scale of the window
     */
    reticle_t(int window_scale);
    reticle_t(const reticle_t&) = delete;
    reticle_t& operator=(const reticle_t&) = delete;
    
    /**
     * Get the target of the reticle in level space
     * Note: this is the inverse operation to the camera conversion
     * @param x the x position in the level set by the call
     * @param y the y position in the level set by the call
     * @param camera the camera
     */
    void get_lvl_target(int& x,
                        int& y,
                        const SDL_Rect& camera) const;

    /**
     * Handle an SDL event (to calculate mouse position)
     */
    void handle_event(const SDL_Event& e);

    /**
     * Render the var
     * @param renderer the renderer to use
     */
    void render(SDL_Renderer& renderer) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_RETICLE_H*/
