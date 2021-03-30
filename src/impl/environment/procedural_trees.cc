/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "procedural_trees.h"
#include "proc_generation.h"
#include "procedural_elem.h"
#include <iostream>

namespace impl {
namespace environment {

  //tree construction values
  #define LEAF_COUNT 200
  #define L_SYSTEM_DEPTH 3
  #define TREE_HEIGHT_MAX 150
  #define TREE_WIDTH_MAX 100
  #define MIN_SPACING 20

  /**
   * Constructor
   * @param region the range in which this tree can be constructed
   * @param renderer the renderer for generating textures
   * @param frame_duration the duration of an animation frame
   */
  procedural_trees_t::procedural_trees_t(const SDL_Rect& region,
                                         SDL_Renderer& renderer,
                                         int frame_duration)
    : procedural_elem_t(region),
      frame_duration(frame_duration),
      anims_fg(),
      positions_fg(),
      anims_bg(),
      positions_bg() {
    this->generate(renderer);
  }

  /**
   * Create a tree in a random position
   * @param constructor the texture constructor
   * @param fg          whether to use foreground colors (vs background)
   * @param x position x
   * @param y position y
   */
  void procedural_trees_t::mk_tree(texture_constructor_t& constructor, bool fg, int x, int y) const {
    int trunk_r = MED_DARK_GREEN_R;
    int trunk_g = MED_DARK_GREEN_G;
    int trunk_b = MED_DARK_GREEN_B;
    int leaf_r = MED_LIGHT_GREEN_R;
    int leaf_g = MED_LIGHT_GREEN_G;
    int leaf_b = MED_LIGHT_GREEN_B;
    if (fg) {
      trunk_r = DARK_R;
      trunk_g = DARK_G;
      trunk_b = DARK_B;
      leaf_r = DARK_GREEN_R;
      leaf_g = DARK_GREEN_G;
      leaf_b = DARK_GREEN_B;
    }

    proc_generation::branching_tree_growth(constructor,
                                           x,y,
                                           trunk_r,
                                           trunk_g,
                                           trunk_b);

    proc_generation::trunk_foliage(constructor,
                                  LEAF_COUNT,
                                  leaf_r,leaf_g,leaf_b,
                                  !fg,
                                  LIGHT_GREEN_R,
                                  LIGHT_GREEN_G,
                                  LIGHT_GREEN_B);
  }

  /**
   * Generate the texture(s)
   * @param renderer the renderer for loading textures
   */
  void procedural_trees_t::generate(SDL_Renderer& renderer) {
    /*
     * Background trees
     */

    int bg_trees = 1;
    int fg_trees = 4;

    int curr_x = bounds.x;

    for (int i=0; i<bg_trees; i++) {
      texture_constructor_t constructor;
      this->mk_tree(constructor,false,TREE_WIDTH_MAX/2,TREE_HEIGHT_MAX);
      int w,h;
      anims_bg.push_back(std::make_unique<entity::anim_set_t>(
        constructor.generate(renderer,w,h),
        w,h,constructor.get_frames(),frame_duration
      ));

      //get the height of the generated tree to root it in the bounding box
      positions_bg.push_back(
        std::make_pair(
          curr_x + (constructor.get_width() / 2),
          bounds.y + bounds.h - (h / 2)));

      curr_x += rand() % constructor.get_width() + MIN_SPACING;
    }

    curr_x = bounds.x;

    /*
     * Foreground trees
     */
     for (int i=0; i<fg_trees; i++) {
       texture_constructor_t constructor;
       this->mk_tree(constructor,true,TREE_WIDTH_MAX/2,TREE_HEIGHT_MAX);
       int w,h;
       anims_fg.push_back(std::make_unique<entity::anim_set_t>(
         constructor.generate(renderer,w,h),
         w,h,constructor.get_frames(),frame_duration
       ));

       //get the height of the generated tree to root it in the bounding box
       positions_fg.push_back(
         std::make_pair(
           curr_x + (constructor.get_width() / 2),
           bounds.y + bounds.h - (h / 2)));

       curr_x += rand() % constructor.get_width() + MIN_SPACING;
     }
  }

  /**
   * Update the renderable component
   */
  void procedural_trees_t::update() {
    for (size_t i=0; i<anims_fg.size(); i++) {
      anims_fg.at(i)->update();
    }
    for (size_t i=0; i<anims_bg.size(); i++) {
      anims_bg.at(i)->update();
    }
  }

  /**
   * Render foreground trees
   * @param renderer the renderer to use
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void procedural_trees_t::render(SDL_Renderer& renderer,
                                  const SDL_Rect& camera,
                                  bool debug) const {
    //check if this region collides with the camera
    if (is_collided(camera,false)) {
      for (size_t i=0; i<anims_fg.size(); i++) {
        //render this tree
        anims_fg.at(i)->render(
          renderer,
          positions_fg.at(i).first - camera.x,
          positions_fg.at(i).second - camera.y,
          false
        );
      }
    }
  }

  /**
   * Render any background trees
   * @param renderer the renderer to use
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void procedural_trees_t::render_bg(SDL_Renderer& renderer,
                                     const SDL_Rect& camera,
                                     bool debug) const {
    //check if this region collides with the camera
    if (is_collided(camera,false)) {
       for (size_t i=0; i<anims_bg.size(); i++) {
         //render this tree
         anims_bg.at(i)->render(
           renderer,
           positions_bg.at(i).first - camera.x,
           positions_bg.at(i).second - camera.y,
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
  }
}}
