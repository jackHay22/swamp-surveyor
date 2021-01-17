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
#include "actions/action.h"
#include "../tilemap/tilemap.h"
#include "../environment/renderable.h"
#include "anim_set.h"

namespace impl {
namespace entity {

  namespace actions {
    class action_t;
  }

  /**
   * Player
   */
  struct player_t : public entity_t {
  private:
    //whether the player is performing an action
    bool performing_action;

    //actions that correspond to the current action type
    std::vector<std::unique_ptr<actions::action_t>> actions;

    //additional player specific animations not covered by the
    //basic entity animations
    std::vector<std::unique_ptr<anim_set_t>> anims;

    //actions that the player can perform
    enum {
      DISPERSE_FOAM
    } action = DISPERSE_FOAM;

  public:
    /**
     * Constructor
     * @param x                  entity position x (center)
     * @param y                  entity position y (center)
     * @param w                  entity bounds width
     * @param h                  entity bounds height
     * @param texture_paths      paths for each of the 4 basic animations
     * @param renderer           the renderer for loading textures
     * @param tile_dim           the dimension of tiles
     */
    player_t(int x, int y,
             int w, int h,
             const std::vector<std::string>& anim_cfg_paths,
             SDL_Renderer& renderer,
             int tile_dim,
             bool debug);
    player_t(const player_t&) = delete;
    player_t& operator=(const player_t&) = delete;

    /**
     * Handle some key event
     * @param e the event
     */
    void handle_event(const SDL_Event& e);

     /**
      * Update the entity (after directional updates)
      * @param map the tilemap
      * @param env_elements environmental elements that can be interacted
      */
     void update(const tilemap::tilemap_t& map,
                 std::vector<std::shared_ptr<environment::renderable_t>>& env_elements);

    /**
     * Render the entity
     * @param renderer the renderer to use
     * @param camera the camera
     */
    void render(SDL_Renderer& renderer, const SDL_Rect& camera) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_PLAYER_H*/
