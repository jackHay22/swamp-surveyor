/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_UI_WINDOW_H
#define _IO_JACKHAY_UI_WINDOW_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <vector>
#include "component.h"

namespace impl {
namespace ui {

  /**
   * A GUI window
   */
  struct window_t : component_t {
  private:
    //the position in the screen
    //and the dimensions
    int x;
    int y;
    int w;
    int h;

    //the cursor position
    int cursor_x;
    int cursor_y;

    //the scale of the surrounding window
    int window_scale;

    //And components in this window
    std::vector<std::unique_ptr<component_t>> subcomponents;

    //the state manager
    state::state_manager_t& state_manager;

  public:
    /**
     * Constructor
     * @param x position x
     * @param y position y
     * @param w width
     * @param h height
     * @param window_scale the scale of the window
     */
    window_t(int x, int y,
             int w, int h,
             int window_scale,
             state::state_manager_t& state_manager);
    window_t(const window_t&) = delete;
    window_t& operator=(const window_t&) = delete;

    /**
     * Add a component to the window
     * @param c the component
     */
    void add_component(std::unique_ptr<component_t> c);

    /**
     * Set the component attributes
     * @param x position x
     * @param y position y
     * @param w dimension w
     * @param h dimension h
     */
    void set_attributes(int x, int y, int w, int h);

    /**
     * Called when a component is clicked
     */
    void clicked() override {}

    /**
     * Once the component is unclicked
     */
    void unclicked(state::state_manager_t&) override {}

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

#endif /*_IO_JACKHAY_UI_WINDOW_H*/
