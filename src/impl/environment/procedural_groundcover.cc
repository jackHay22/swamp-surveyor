/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "procedural_groundcover.h"
#include "proc_generation.h"
#include "procedural_elem.h"

namespace impl {
namespace environment {

  #define BUSH_SPACING 50
  #define BUSH_BUFFER 15

  /**
   * Constructor
   * @param region         the region to populate
   * @param renderer       the renderer for generating textures
   * @param frame_duration the duration of each animation frame
   */
  procedural_groundcover_t::procedural_groundcover_t(const SDL_Rect& region,
                           SDL_Renderer& renderer,
                           int frame_duration)
    : procedural_elem_t(region),
      frame_duration(frame_duration) {
    this->generate(renderer);
  }


  /**
   * Populate the foreground or the background texture
   * @param constructor the texture constructor
   * @param fg          whether to use foreground colors (vs background)
   */
  void procedural_groundcover_t::populate_region(texture_constructor_t& constructor,
                                                 bool fg) const {
   int grass_r = MED_LIGHT_GREEN_R;
   int grass_g = MED_LIGHT_GREEN_G;
   int grass_b = MED_LIGHT_GREEN_B;

   if (fg) {
     grass_r = DARK_GREEN_R;
     grass_g = DARK_GREEN_G;
     grass_b = DARK_GREEN_B;
   }

   //generate grass


   //generate bushes
   int bush_interval = (bounds.w / BUSH_SPACING);
   int bush_count = rand() % std::max(bush_interval,1) + 1;
   int bush_x = BUSH_BUFFER;

   for (int b=0; b<bush_count; b++) {
     //use the lsystem
     proc_generation::fractal_tree_lsystem(
       constructor,
       3,
       grass_r, grass_g, grass_b,
       bush_x,
       bounds.y
     );

     bush_x += rand() % BUSH_SPACING + BUSH_BUFFER;
   }
  }

  /**
   * Generate the texture(s)
   * @param renderer the renderer for loading textures
   */
  void procedural_groundcover_t::generate(SDL_Renderer& renderer) {
    int w,h;
    //generate the foreground region
    texture_constructor_t fg_constructor;
    populate_region(fg_constructor,true);

    //create the animation
    anim_fg = std::make_unique<entity::anim_set_t>(
      fg_constructor.generate(renderer,w,h),
      w,h,fg_constructor.get_frames(),
      frame_duration
    );

    //generate the background region
    texture_constructor_t bg_constructor;
    populate_region(bg_constructor,false);

    //create the animation
    anim_bg = std::make_unique<entity::anim_set_t>(
      bg_constructor.generate(renderer,w,h),
      w,h,bg_constructor.get_frames(),
      frame_duration
    );
  }

  /**
   * Update the renderable component (animations)
   */
  void procedural_groundcover_t::update() {
    anim_fg->update();
    anim_bg->update();
  }

  /**
   * Render foreground components
   * @param renderer the renderer to use
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void procedural_groundcover_t::render(SDL_Renderer& renderer,
                                        const SDL_Rect& camera,
                                        bool debug) const {
    if (is_collided(camera,false)) {
      //render the background
      anim_fg->render(
        renderer,
        bounds.x + (bounds.w / 2) - camera.x,
        bounds.y + (bounds.h / 2) - camera.y,
        false
      );
    }
  }

  /**
   * Render any background components
   * @param renderer the renderer to use
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void procedural_groundcover_t::render_bg(SDL_Renderer& renderer,
                                           const SDL_Rect& camera,
                                           bool debug) const {
    if (is_collided(camera,false)) {
      //render the background
      anim_bg->render(
        renderer,
        bounds.x + (bounds.w / 2) - camera.x,
        bounds.y + (bounds.h / 2) - camera.y,
        false
      );
    }

    if (debug) {
      SDL_Rect debug_bounds = {bounds.x - camera.x,
                               bounds.y - camera.y,
                               bounds.w, bounds.h};

      //set the draw color
      SDL_SetRenderDrawColor(&renderer,255,102,0,255);

      //render the bounds
      SDL_RenderDrawRect(&renderer,&debug_bounds);
    }
  }
}}
