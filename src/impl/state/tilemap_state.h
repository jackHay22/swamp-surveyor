/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TILEMAP_STATE_H
#define _IO_JACKHAY_SWAMP_TILEMAP_STATE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "state.h"
#include <memory>
#include <vector>
#include "../tilemap/tilemap.h"
#include "../entity/entity.h"
#include "../entity/player.h"

namespace impl {
namespace state {

  /**
   * The main tilemap state
   */
  struct tilemap_state_t : public state_t {
  private:
    //the game tilemap
    std::shared_ptr<tilemap::tilemap_t> tilemap;

    //the loaded entities
    std::vector<std::shared_ptr<entity::entity_t>> entities;

    //the index of the player in the entity list
    std::shared_ptr<entity::player_t> player;

  public:
    //constructor
    tilemap_state_t(std::shared_ptr<tilemap::tilemap_t> tilemap,
                    std::vector<std::shared_ptr<entity::entity_t>>& entities,
                    int player_idx, SDL_Rect& camera);
    tilemap_state_t(const tilemap_state_t&) = delete;
    tilemap_state_t& operator=(const tilemap_state_t&) = delete;

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

#endif /*_IO_JACKHAY_SWAMP_TILEMAP_STATE_H*/
