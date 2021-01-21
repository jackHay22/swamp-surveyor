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
#include "state_manager.h"
#include <memory>
#include <vector>
#include "../tilemap/tilemap.h"
#include "../tilemap/transparent_block.h"
#include "../entity/entity.h"
#include "../entity/player.h"
#include "../entity/insects.h"
#include "../entity/indicator_bar.h"
#include "../environment/renderable.h"
#include "../items/item.h"
#include "../misc/map_fork.h"

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

    //insects in the map
    std::shared_ptr<entity::insects_t> insects;

    //renderable environment components
    std::vector<std::shared_ptr<environment::renderable_t>> env_renderable;

    //items in the level
    std::vector<std::shared_ptr<items::item_t>> level_items;

    //transparent blocks in the level
    std::vector<std::shared_ptr<tilemap::transparent_block_t>> trans_blocks;

    //forks in the map
    std::vector<std::shared_ptr<misc::map_fork_t>> forks;

    //whether indicator bars are visible
    bool show_bars;
    //the indicator bar showing player health
    entity::indicator_bar_t player_health_bar;

    /**
     * Check if a given bounding box is on solid ground
     * @param  bounds the bounding box
     * @return        whether the box is against a solid component
     */
    bool on_solid_ground(const SDL_Rect& bounds) const;

    /**
     * Check if an entity has collided with a solid environmental object
     * @param  bounds the bounds of the entity
     * @return        whether the entity collided with the env object
     */
    bool is_collided_solid_env(const SDL_Rect& bounds) const;

  public:
    /**
     * Constructor
     * @param tilemap    the tilemap this state uses
     * @param entities   the entities in this state
     * @param insects    the insects in the map
     * @param env_renderable environmental elements that can be rendered
     * @param level_items the items in this level
     * @param trans_blocks transparent blocks in the level
     * @param forks      forks in the map
     * @param player_idx the index of the player in the entity list
     * @param manager    the state manager
     * @param camera     the level camera
     */
    tilemap_state_t(std::shared_ptr<tilemap::tilemap_t> tilemap,
                    std::vector<std::shared_ptr<entity::entity_t>>& entities,
                    std::shared_ptr<entity::insects_t> insects,
                    std::vector<std::shared_ptr<environment::renderable_t>>& env_renderable,
                    std::vector<std::shared_ptr<items::item_t>>& level_items,
                    std::vector<std::shared_ptr<tilemap::transparent_block_t>>& trans_blocks,
                    std::vector<std::shared_ptr<misc::map_fork_t>>& forks,
                    int player_idx, state_manager_t& manager,
                    SDL_Rect& camera);
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
