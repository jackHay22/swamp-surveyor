/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_PLAYER_H
#define _IO_JACKHAY_SWAMP_PLAYER_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <string>
#include "entity.h"

namespace impl {
namespace entity {

  /**
   * Player
   */
  struct player_t : public entity_t {
  private:

  public:
    /**
     * Constructor
     * @param x                  entity position x (center)
     * @param y                  entity position y (center)
     * @param w                  entity bounds width
     * @param h                  entity bounds height
     * @param texture_paths      paths for each of the 4 basic animations
     * @param renderer           the renderer for loading textures
     */
    player_t(int x, int y,
             int w, int h,
             const std::vector<std::string>& anim_cfg_paths,
             SDL_Renderer& renderer,
             bool debug);
    player_t(const player_t&) = delete;
    player_t& operator=(const player_t&) = delete;

    /**
     * Handle some key event
     * @param e the event
     */
    void handle_event(const SDL_Event& e);

    /**
     * Update the entity
     */
    void update();

    /**
     * Render the entity
     * @param renderer the renderer to use
     * @param camera the camera
     */
    void render(SDL_Renderer& renderer, const SDL_Rect& camera) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_PLAYER_H*/
