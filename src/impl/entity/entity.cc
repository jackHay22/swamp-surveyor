/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "entity.h"
#include "../exceptions.h"

namespace impl {
namespace entity {

  /**
   * Constructor
   * @param x                  entity position x (center)
   * @param y                  entity position y (center)
   * @param w                  entity bounds width
   * @param h                  entity bounds height
   * @param anim_cfg_paths     the paths to the configuration files for each animation
   * @param renderer           the renderer for loading textures
   */
  entity_t::entity_t(int x, int y,
                     int w, int h,
                     const std::vector<std::string>& anim_cfg_paths,
                     SDL_Renderer& renderer)
    : x(x), y(y), w(w), h(h), last_x(), last_y(), anims() {

      if (anim_cfg_paths.size() < 4) {
        throw exceptions::rsrc_exception_t("not enough entity animation paths provided");
      }

      //load animation frames (must be in order)
      for (const std::string& p : anim_cfg_paths) {
        //create the animation set and add to list
        anims.push_back(std::make_unique<anim_set_t>(p,renderer));
      }
    }

  /**
   * Get the bounding box for this entity
   * @return the bounding box for this entity
   */
  SDL_Rect entity_t::get_bounds() const {
    //generate bounding box
    SDL_Rect r = {x - (w / 2),
                  y - (h / 2),
                  x + (w / 2),
                  y + (h / 2)};
    return r;
  }

  /**
   * Update this entity at the tick
   */
  void entity_t::update() {
    //TODO update position

    //update the current animation
    anims.at(state)->update();
  }

  /**
   * Restore the previous position of the entity
   */
  void entity_t::step_back() {
    this->x = this->last_x;
    this->y = this->last_y;
  }

  /**
   * Render the entity
   * @param renderer the renderer to use
   */
  void entity_t::render(SDL_Renderer& renderer) const {
    //render the current animation
    anims.at(state)->render(renderer, x - (w / 2), y - (h / 2));
  }
}}
