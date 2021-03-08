/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "map.h"
#include "colors.h"
#include "../utils.h"

namespace impl {
namespace ui {

  /**
   * Constructor
   * @param map_texture_path the path to the map texture
   * @param renderer the renderer to load the font
   */
  map_t::map_t(const std::string& map_texture_path,
               SDL_Renderer& renderer)
    : x(0), y(0),
      texture(NULL),
      texture_w(0),
      texture_h(0),
      was_clicked(false) {
    //Load the map texture
    texture = utils::load_texture(map_texture_path,
                                  renderer,
                                  texture_w,
                                  texture_h);
  }

  /**
   * Destructor to free texture
   */
  map_t::~map_t() {
    // free the SDL texture
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }

  /**
   * Set the component attributes
   * @param x position x
   * @param y position y
   * @param w dimension w
   * @param h dimension h
   */
  void map_t::set_attributes(int x, int y, int /*w*/, int /*h*/) {
    //ignores the width and height (not dynamically sized)
    this->x = x;
    this->y = y;
  }

  /**
   * Called when a component is clicked
   */
  void map_t::clicked() {
    this->was_clicked = true;
  }

  /**
   * Called when the component is unclicked
   */
  void map_t::unclicked(state::state_manager_t& manager) {
    if (was_clicked) {
      //some operation

      was_clicked = false;
    }
  }

  /**
   * Whether a given x y position is in bounds
   * @param  x position x
   * @param  y position y
   * @return   whether this position is in bounds
   */
  bool map_t::in_bounds(int x, int y) const {
    return (x > this->x) &&
           (x < (this->x + texture_w)) &&
           (y > this->y) &&
           (y < (this->y + texture_h));
  }

  /**
   * Get the bounds of this component
   * @return the bounds of the component on the screen
   */
  SDL_Rect map_t::bounds() const {
    return {x,y,texture_w,texture_h};
  }

  /**
   * Handle an SDL event (to calculate mouse position)
   */
  void map_t::handle_event(const SDL_Event& e) {
    //TODO
  }

  /**
   * Render the var
   * @param renderer the renderer to use
   */
  void map_t::render(SDL_Renderer& renderer) const {

    //ignore if no texture
    if ((texture_h == 0) || (texture_w == 0)) {
      return;
    }

    //render the background frame
    SDL_SetRenderDrawColor(&renderer,R_DARK,G_DARK,B_DARK,255);

    //frame is slightly bigger than texture
    SDL_Rect rect = {x-1,y-1,texture_w + 2,texture_h + 2};

    //render the bounds
    SDL_RenderDrawRect(&renderer,&rect);

    SDL_SetRenderDrawColor(&renderer,R_HIGHLIGHT,G_HIGHLIGHT,B_HIGHLIGHT,255);

    //render the highlight
    //horiz
    SDL_RenderDrawLine(&renderer,
                       x,
                       y+texture_h,
                       x+texture_w,
                       y+texture_h);

    //vert
    SDL_RenderDrawLine(&renderer,
                       x+texture_w,
                       y,
                       x+texture_w,
                       y+texture_h-1);

    //render the texture
    //create a clip for the current frame
    SDL_Rect sample_bounds = {0,0,texture_w,texture_h};

    //the x y position to render at
    SDL_Rect image_bounds = {x,y,texture_w,texture_h};

    //render the texture
    SDL_RenderCopy(&renderer,
                   texture,
                   &sample_bounds,
                   &image_bounds);
  }

}}
