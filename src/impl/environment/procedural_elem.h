/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_PROCEDURAL_ELEM_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_PROCEDURAL_ELEM_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "renderable.h"

namespace impl {
namespace environment {

  //COLORS
  #define LIGHT_GREEN_R 125
  #define LIGHT_GREEN_G 174
  #define LIGHT_GREEN_B 109
  #define MED_LIGHT_GREEN_R 104
  #define MED_LIGHT_GREEN_G 156
  #define MED_LIGHT_GREEN_B 91
  #define MED_DARK_GREEN_R 72
  #define MED_DARK_GREEN_G 110
  #define MED_DARK_GREEN_B 67
  #define DARK_GREEN_R 41 //FG leaf
  #define DARK_GREEN_G 58
  #define DARK_GREEN_B 0
  #define DARK_R 8 //FG trunk
  #define DARK_G 17
  #define DARK_B 0

  /**
   * A procedurally generated element in the environment
   */
  struct procedural_elem_t : public renderable_t {
  public:
    //Constructor takes the procedurally generated region
    procedural_elem_t(const SDL_Rect& region) : renderable_t(region,false,false) {}
    procedural_elem_t(const procedural_elem_t&) = delete;
    procedural_elem_t& operator=(const procedural_elem_t&) = delete;

    /**
     * Generate the texture(s)
     * @param renderer the renderer for loading textures
     */
    virtual void generate(SDL_Renderer& renderer) = 0;

    /**
     * Procedural elements non interactive
     * @return whether this element can be interacted with by the player
     */
    bool is_interactive() const { return false; }

    /**
     * Procedural elements not solid
     * @return whether this element is solid
     */
    bool is_solid() const { return false; }

    /**
     * No damage done by procedural elements
     * @return the damage this environment element does
     */
    int get_damage() const { return 0; }

    /**
     * Update the renderable component
     */
    virtual void update() override = 0;

    /**
     * Render any background components
     * @param renderer the renderer to use
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    virtual void render(SDL_Renderer& renderer,
                        const SDL_Rect& camera,
                        bool debug) const override = 0;

    /**
     * Render any background components
     * @param renderer the renderer to use
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    virtual void render_bg(SDL_Renderer& renderer,
                           const SDL_Rect& camera,
                           bool debug) const = 0;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_PROCEDURAL_ELEM_H*/
