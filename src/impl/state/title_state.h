/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TITLE_STATE_H
#define _IO_JACKHAY_SWAMP_TITLE_STATE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "state.h"
#include <string>

namespace impl {
namespace state {

  /**
   * The starting state displaying options
   */
  struct title_state_t : public state_t {
  private:
    int width;
    int height;

    //background image texture
    SDL_Texture* texture = NULL;
  public:
    //constructor
    title_state_t(int width,
                  int height,
                  const std::string& bg_path,
                  SDL_Renderer& renderer);
    title_state_t(const title_state_t&) = delete;
    title_state_t& operator=(const title_state_t&) = delete;

    /**
     * Handle some key event
     * @param e the event
     */
    void handle_event(const SDL_Event& e);

    /**
     * Update this tile
     */
    void update();

    /**
     * Render the current gamestate
     * @param renderer the renderer
     */
    void render(SDL_Renderer& renderer) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_TITLE_STATE_H*/
