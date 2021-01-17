/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_FOAM_SPRAY_H
#define _IO_JACKHAY_SWAMP_FOAM_SPRAY_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <vector>
#include <utility>
#include <memory>
#include "action.h"
#include "../../environment/renderable.h"

namespace impl {
namespace entity {
namespace actions {

  /**
   * Defines the foam that a player uses to disperse foam
   */
  struct foam_spray_t : public action_t {
  private:
    //the spray particles
    //x, y coordinates and finally the number of ticks
    //before the next update
    std::vector<std::pair<int,int>> particles;

  public:
    foam_spray_t();
    foam_spray_t(const foam_spray_t&) = delete;
    foam_spray_t& operator=(const foam_spray_t&) = delete;

    /**
     * Update the action
     * @param map the tilemap
     * @param env_elements environmental elements that this action
     *  can interact with
     *  @param x  the position where spray should emit from x
     *  @param y  the position where spray should emit from y
     *  @param dir the direction (either 1 or -1)
     */
    void update(const tilemap::tilemap_t& map,
                std::vector<std::shared_ptr<environment::renderable_t>>& env_elements,
                int x, int y, int dir);

    /**
     * Render the action
     * @param renderer the renderer to use
     * @param camera the camera
     */
    void render(SDL_Renderer& renderer, const SDL_Rect& camera) const;
  };
}}}

#endif /*_IO_JACKHAY_SWAMP_FOAM_SPRAY_H*/
