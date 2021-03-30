/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_PROCEDURAL_GROUND_COVER_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_PROCEDURAL_GROUND_COVER_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <memory>
#include "procedural_elem.h"
#include "../entity/anim_set.h"
#include "texture_constructor.h"

namespace impl {
namespace environment {

  /**
   * A procedurally generated region of ground cover
   */
  struct procedural_groundcover_t : public procedural_elem_t {
  private:
    //the delay between animation frames
    int frame_duration;
    //foreground ground cover animation
    std::unique_ptr<entity::anim_set_t> anim_fg;

    //background ground cover animation
    std::unique_ptr<entity::anim_set_t> anim_bg;

    /**
     * Populate the foreground or the background texture
     * @param constructor the texture constructor
     * @param fg          whether to use foreground colors (vs background)
     */
    void populate_region(texture_constructor_t& constructor,
                         bool fg) const;

  public:
    procedural_groundcover_t(const SDL_Rect& region,
                             SDL_Renderer& renderer,
                             int frame_duration);
    procedural_groundcover_t(const procedural_groundcover_t&) = delete;
    procedural_groundcover_t& operator=(const procedural_groundcover_t&) = delete;

    /**
     * Generate the texture(s)
     * @param renderer the renderer for loading textures
     */
    void generate(SDL_Renderer& renderer) override;

    /**
     * Update the renderable component (animations)
     */
    void update() override;

    /**
     * Render foreground components
     * @param renderer the renderer to use
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    void render(SDL_Renderer& renderer,
                const SDL_Rect& camera,
                bool debug) const override;

    /**
     * Render any background components
     * @param renderer the renderer to use
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    void render_bg(SDL_Renderer& renderer,
                   const SDL_Rect& camera,
                   bool debug) const override;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_PROCEDURAL_GROUND_COVER_H*/
