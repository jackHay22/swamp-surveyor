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

    //positions restored on collision
    int last_x;
    int last_y;

    //texture animations
    std::vector<std::unique_ptr<anim_set_t>> anims;

  protected:
    //the current state of the entity, can be set by subclasses
    enum {
      IDLE_LEFT,
      IDLE_RIGHT,
      MOVE_LEFT,
      MOVE_RIGHT
    } state = MOVE_RIGHT;

  public:
    /**
     * Constructor
     * @param x                  entity position x (center)
     * @param y                  entity position y (center)
     * @param w                  entity bounds width
     * @param h                  entity bounds height
     * @param anim_cfg_paths     the paths to the configuration files for each animation
     * @param renderer           the renderer for loading textures
     */
    entity_t(int x, int y,
             int w, int h,
             const std::vector<std::string>& anim_cfg_paths,
             SDL_Renderer& renderer);
    entity_t(const entity_t&) = delete;
    entity_t& operator=(const entity_t&) = delete;

    virtual ~entity_t() {}

    /**
     * Get the bounding box for this entity
     * @return the bounding box for this entity
     */
    SDL_Rect get_bounds() const;

    /**
     * Update this entity at the tick
     */
    void update();

    /**
     * Restore the previous position of the entity
     */
    void step_back();

    /**
     * Render the entity
     * @param renderer the renderer to use
     */
    void render(SDL_Renderer& renderer) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENTITY_H*/
