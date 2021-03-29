/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_PROCEDURAL_TREES_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_PROCEDURAL_TREES_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <utility>
#include "procedural_elem.h"
#include "../entity/anim_set.h"
#include "texture_constructor.h"

namespace impl {
namespace environment {

  /**
   * A procedurally generated element in the environment
   */
  struct procedural_trees_t : public procedural_elem_t {
  private:
    //the delay between animation frames
    int frame_duration;
    //foreground tree animations (for generating some movement in the leaves)
    std::vector<std::unique_ptr<entity::anim_set_t>> anims_fg;
    //foreground positions in the region
    std::vector<std::pair<int,int>> positions_fg;

    //background tree animations (for generating some movement in the leaves)
    std::vector<std::unique_ptr<entity::anim_set_t>> anims_bg;
    //background positions in the region
    std::vector<std::pair<int,int>> positions_bg;

    /**
     * Create a tree in a random position
     * @param constructor the texture constructor
     * @param fg          whether to use foreground colors (vs background)
     * @param x position x
     * @param y position y
     */
    void mk_tree(texture_constructor_t& constructor,
                 bool fg,
                 int x,
                 int y) const;

  public:
    procedural_trees_t(const SDL_Rect& region,
                       SDL_Renderer& renderer,
                       int frame_duration);
    procedural_trees_t(const procedural_trees_t&) = delete;
    procedural_trees_t& operator=(const procedural_trees_t&) = delete;

    /**
     * Generate the texture(s)
     * @param renderer the renderer for loading textures
     */
    void generate(SDL_Renderer& renderer) override;

    /**
     * Update the renderable component
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

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_PROCEDURAL_TREES_H*/
