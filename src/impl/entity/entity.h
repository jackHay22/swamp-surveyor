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
#include <functional>
#include "anim_set.h"
#include "../tilemap/tilemap.h"
#include "../environment/environment.h"

namespace impl {
namespace entity {

  //the number of states the entity class controls
  #define ENTITY_STATES 4

  /**
   * Types of npcs
   */
  enum npc_type {
    NON_NPC, // i.e. the player
    SURVEYOR
  };

  /*
   * The state of an entity
   */
  enum entity_state {
    IDLE,
    MOVE,
    CLIMB,
    DROP,
    ACTION
  };

  /*
   * The position of an entity. A vector of these is
   * passed to an entity on update including some map information
   * The integers correspond to position, the first boolean value is
   * true iff the entity is the player
   * The second boolean value is true iff this entity is self
   * The final value is the type of the entity
   */
  typedef std::tuple<int,int,bool,bool,npc_type> entity_pos_t;
  #define EPOS_X 0
  #define EPOS_Y 1
  #define EPOS_PLAYER 2
  #define EPOS_SELF 3
  #define EPOS_TYPE 4

  /*
   * A behavior handler
   */
  typedef std::function<void(const std::vector<entity_pos_t>&,
                             const tilemap::tilemap_t&,
                             int, int,
                             entity_state&,
                             bool&)> behavior_handler_t;

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
    //the height the player needs to climb
    int climb_height;

    //the counter for skipping ticks in water
    int water_counter;

    //the dimension of tiles
    int tile_dim;

    //texture animations (8 -- actions are covered by subclasses)
    std::vector<std::unique_ptr<anim_set_t>> anims;

    //the update behavior of this entity (used for non-players)
    behavior_handler_t on_behavior;

    //the type of this entity (used for behavior)
    npc_type entity_type;

  protected:
    //whether the entity is facing left (vs right)
    bool facing_left;

    //the current state of the entity, can be set by subclasses
    //previous state used for restoring last state
    entity_state state = IDLE, prev_state = IDLE;

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
     * @param base_path          the resource directory base path
     * @param on_behavior        the behavior handler
     * @param entity_type        the type of this entity
     */
    entity_t(int x, int y,
             int w, int h,
             const std::vector<std::string>& anim_cfg_paths,
             SDL_Renderer& renderer,
             int tile_dim,
             const std::string& base_path,
             behavior_handler_t on_behavior,
             npc_type entity_type);
    entity_t(const entity_t&) = delete;
    entity_t& operator=(const entity_t&) = delete;

    virtual ~entity_t() {}

    /**
     * Get the npc type
     * @return the npc type of this entity
     */
    npc_type get_npc_type() const { return entity_type; }

    /**
     * Check whether the entity is facing left
     * @return whether the entity is facing left
     */
    bool is_facing_left() const { return facing_left; }

    /**
     * Set the entity position
     * @param x x coord for entity
     * @param y y coord for entity
     */
    void set_position(int x, int y) { this->x = x; this->y = y; }

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
     * @param env the environment
     */
    virtual void update(const tilemap::tilemap_t& map,
                        environment::environment_t& env);

    /**
     * Update this entity at the tick in the x direction
     */
    void update_x(const tilemap::tilemap_t& map);

    /**
     * Update this entity at the tick in the y direction
     */
    void update_y();

    /**
     * Update the behavior of this entity given other entity positions
     * and the tilemap
     * @param entity_pos the positions of all entities in the map
     * @param map        the tilemap
     */
    virtual void update_behavior(const std::vector<entity_pos_t>& entity_pos,
                                 const tilemap::tilemap_t& map);

    /**
     * Called when entity collides in the x direction
     * @param map used to determine if the entity can climb
     * @param env used to determine if the entity can climb
     */
    void step_back_x(const tilemap::tilemap_t& map,
                     const environment::environment_t& env);

    /**
     * Restore the position of the entity (y dir)
     */
    void step_back_y();

    /**
     * Render the entity
     * @param renderer the renderer to use
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    virtual void render(SDL_Renderer& renderer,
                        const SDL_Rect& camera,
                        bool debug) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENTITY_H*/
