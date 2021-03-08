/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_UI_MAP_H
#define _IO_JACKHAY_UI_MAP_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <functional>
#include <string>
#include "component.h"
#include "../state/state_manager.h"

namespace impl {
namespace ui {

  /**
   * A GUI map
   */
  struct map_t : component_t {
  private:
    //the position in the screen
    int x;
    int y;

    //the map texture
    SDL_Texture *texture;
    int texture_w;
    int texture_h;

    //related to the click animation
    bool was_clicked;

  public:
    /**
     * Constructor
     * @param map_texture_path the path to the map texture
     * @param renderer the renderer to load the font
     */
    map_t(const std::string& map_texture_path,
             SDL_Renderer& renderer);
    map_t(const map_t&) = delete;
    map_t& operator=(const map_t&) = delete;

    /**
     * Destructor to free texture
     */
    ~map_t();

    /**
     * Get the default height of a component
     * @return the default height
     */
    int get_default_height() const override { return texture_h; };

    /**
     * Set the component attributes
     * @param x position x
     * @param y position y
     * @param w dimension w
     * @param h dimension h
     */
    void set_attributes(int x, int y, int w, int h) override;

    /**
     * Called when a component is clicked
     */
    void clicked() override;

    /**
     * Called when the component is unclicked
     */
    void unclicked(state::state_manager_t& manager) override;

    /**
     * Whether a given x y position is in bounds
     * @param  x position x
     * @param  y position y
     * @return   whether this position is in bounds
     */
    bool in_bounds(int x, int y) const override;

    /**
     * Get the bounds of this component
     * @return the bounds of the component on the screen
     */
    SDL_Rect bounds() const override;

    /**
     * Handle an SDL event (to calculate mouse position)
     */
    void handle_event(const SDL_Event& e) override;

    /**
     * Render the var
     * @param renderer the renderer to use
     */
    void render(SDL_Renderer& renderer) const override;
  };
}}

#endif /*_IO_JACKHAY_UI_MAP_H*/
