/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ACTIONS_ACTION_H
#define _IO_JACKHAY_SWAMP_ACTIONS_ACTION_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include "../../environment/renderable.h"
#include "../../tilemap/tilemap.h"

namespace impl {
namespace entity {
namespace actions {

  /**
   * Abstract player action
   */
  struct action_t {
  protected:
    //whether this action is active (i.e. generating new particles)
    bool active;
    //whether this action should be rendered/updated
    //(i.e. animating remaining particles)
    bool visible;

  public:
    action_t() : active(false), visible(false) {}
    action_t(const action_t&) = delete;
    action_t& operator=(const action_t&) = delete;

    /**
     * Toggle the action on/off
     */
    void toggle_action(bool active) { this->active = active; }

    /**
     * Update the action
     * @param map the tilemap
     * @param env_elements environmental elements that this action
     *  can interact with
     * @param x action position x
     * @param y action position y
     * @param dir the direction
     */
    virtual void update(const tilemap::tilemap_t& map,
                        std::vector<std::shared_ptr<environment::renderable_t>>& env_elements,
                        int x, int y, int dir) {}

    /**
     * Render the action
     * @param renderer the renderer to use
     * @param camera the camera
     */
    virtual void render(SDL_Renderer& renderer, const SDL_Rect& camera) const {}
  };
}}}

#endif /*_IO_JACKHAY_SWAMP_ACTIONS_ACTION_H*/
