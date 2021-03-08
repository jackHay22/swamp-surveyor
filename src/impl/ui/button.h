/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_UI_BUTTON_H
#define _IO_JACKHAY_UI_BUTTON_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <functional>
#include <string>
#include "component.h"
#include "../state/state_manager.h"

namespace impl {
namespace ui {

  #define DEFAULT_BUTTON_HEIGHT 15

  /**
   * A GUI button
   */
  struct button_t : component_t {
  private:
    //the position in the screen
    //and the dimensions
    int x;
    int y;
    int w;
    int h;

    //the label texture
    SDL_Texture *texture;
    int texture_w;
    int texture_h;

    //related to the click animation
    bool was_clicked;

    //handler for button click
    std::function<void(state::state_manager_t&)> handler;

  public:
    /**
     * Constructor
     * @param text         the button text
     * @param window_scale the scale of the window
     * @param font_path the path to the font to use
     * @param renderer the renderer to load the font
     */
    button_t(const std::string& text,
             std::function<void(state::state_manager_t& manager)> handler,
             const std::string& font_path,
             SDL_Renderer& renderer);
    button_t(const button_t&) = delete;
    button_t& operator=(const button_t&) = delete;

    /**
     * Destructor to free texture
     */
    ~button_t();

    /**
     * Get the default height of a component
     * @return the default height
     */
    int get_default_height() const override { return DEFAULT_BUTTON_HEIGHT; };

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
    void clicked(state::state_manager_t& manager) override;

    /**
     * Called when the component is unclicked
     */
    void unclicked() override;

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

#endif /*_IO_JACKHAY_UI_BUTTON_H*/
