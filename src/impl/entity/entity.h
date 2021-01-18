/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENTITY_H
#define _IO_JACKHAY_SWAMP_ENTITY_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <string>
#include "anim_set.h"
#include "../tilemap/tilemap.h"
#include "../environment/renderable.h"

namespace impl {
namespace entity {

  /**
   * Positional entity with 4 animations
   */
  struct entity_t {
  private:
    //the location and bounds of entity
    //The location corresponds to the center of the entity
    int x;
    int y;
    int w;
    int h;

    //the amount of health the player has
    int health;
    //the ticks remaining to show damage
    int damaged_ticks;

    //positions restored on collision
    int last_x;
    int last_y;

    //the counter during a climb cycle
    int climb_counter;

    //the counter for skipping ticks in water
    int water_counter;

    //the dimension of tiles
    int tile_dim;

    //texture animations (8 -- actions are covered by subclasses)
    std::vector<std::unique_ptr<anim_set_t>> anims;

  protected:
    //whether the entity is facing left (vs right)
    bool facing_left;

    //whether debug mode enabled
    bool debug;

    //the current state of the entity, can be set by subclasses
    enum {
      IDLE,
      MOVE,
      CLIMB,
      DROP,
      ACTION,
    } state = IDLE;

  public:
    /**
     * Constructor
     * @param x                  entity position x (center)
     * @param y                  entity position y (center)
     * @param w                  entity bounds width
     * @param h                  entity bounds height
     * @param anim_cfg_paths     the paths to the configuration files for each animation
     * @param renderer           the renderer for loading textures
     * @param tile_dim           the dimensions of tiles
     */
    entity_t(int x, int y,
             int w, int h,
             const std::vector<std::string>& anim_cfg_paths,
             SDL_Renderer& renderer,
             int tile_dim,
             bool debug);
    entity_t(const entity_t&) = delete;
    entity_t& operator=(const entity_t&) = delete;

    virtual ~entity_t() {}

    /**
     * Get the entity health remaining
     * @return the entity health
     */
    int get_health() const { return health; }

    /**
     * Do damage to this entity by some magnitude
     * @param magnitude the amount off damage to do
     */
    void do_damage(int magnitude);

    /**
     * Get the bounding box for this entity
     * @return the bounding box for this entity
     */
    SDL_Rect get_bounds() const;

    /**
     * Get the center of the entity
     * @param x the x position set by the call
     * @param y the y position set by the call
     */
    void get_center(int& x, int& y) const;

    /**
     * Update the entity (after directional updates)
     * @param map the tilemap
     * @param env_elements environmental elements that can be interacted
     */
    virtual void update(const tilemap::tilemap_t& map,
                        std::vector<std::shared_ptr<environment::renderable_t>>& env_elements);

    /**
     * Update this entity at the tick in the x direction
     */
    void update_x(const tilemap::tilemap_t& map);

    /**
     * Update this entity at the tick in the y direction
     */
    void update_y();

    /**
     * Called when entity collides in the x direction
     * @param layer used to determine if the entity can step up
     */
    void step_back_x(const tilemap::tilemap_t& map);

    /**
     * Restore the position of the entity (y dir)
     */
    void step_back_y();

    /**
     * Render the entity
     * @param renderer the renderer to use
     * @param camera   the camera
     */
    virtual void render(SDL_Renderer& renderer, const SDL_Rect& camera) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENTITY_H*/
