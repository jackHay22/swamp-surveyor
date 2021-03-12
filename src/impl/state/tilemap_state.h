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
#include "../environment/environment.h"
#include "../items/item.h"
#include "../misc/map_fork.h"
#include "../entity/reticle.h"

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
    //the player index in the entity list
    int player_idx;

    //insects in the map
    std::shared_ptr<entity::insects_t> insects;

    //the environment in the tilemap
    std::shared_ptr<environment::environment_t> env;

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

    //the mouse position
    std::unique_ptr<entity::reticle_t> reticle;

    //the name of the cfg file for this state
    std::string cfg_name;

    /**
     * Check if a given bounding box is on solid ground
     * @param  bounds the bounding box
     * @return        whether the box is against a solid component
     */
    bool on_solid_ground(const SDL_Rect& bounds) const;

  public:
    /**
     * Constructor
     * @param tilemap    the tilemap this state uses
     * @param entities   the entities in this state
     * @param insects    the insects in the map
     * @param env        environmental elements in the state
     * @param level_items the items in this level
     * @param trans_blocks transparent blocks in the level
     * @param forks      forks in the map
     * @param player_idx the index of the player in the entity list
     * @param manager    the state manager
     * @param camera     the level camera
     * @param cfg_name   the local path (in resources dir) to the file that configures this state
     */
    tilemap_state_t(std::shared_ptr<tilemap::tilemap_t> tilemap,
                    std::vector<std::shared_ptr<entity::entity_t>>& entities,
                    std::shared_ptr<entity::insects_t> insects,
                    std::shared_ptr<environment::environment_t> env,
                    std::vector<std::shared_ptr<items::item_t>>& level_items,
                    std::vector<std::shared_ptr<tilemap::transparent_block_t>>& trans_blocks,
                    std::vector<std::shared_ptr<misc::map_fork_t>>& forks,
                    int player_idx, state_manager_t& manager,
                    SDL_Rect& camera,
                    const std::string& cfg_name);
    tilemap_state_t(const tilemap_state_t&) = delete;
    tilemap_state_t& operator=(const tilemap_state_t&) = delete;

    /**
     * Get the local path (in resources dir) to the file that configures this state
     * @return the cfg name
     */
    const std::string& get_cfg() const { return cfg_name; }

    /**
     * Set the player
     * @param player the player to add to state
     */
    void set_player(std::shared_ptr<entity::player_t> player);

    /**
     * Get the player
     */
    std::shared_ptr<entity::player_t> get_player() { return player; }

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
     * @param debug    whether debug mode enabled
     */
    void render(SDL_Renderer& renderer, bool debug) const;

    /**
     * Render any debug info
     * @param renderer sdl renderer
     * @param font     loaded ttf font
     */
    void render_debug_info(SDL_Renderer& renderer, TTF_Font& font) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_TILEMAP_STATE_H*/
