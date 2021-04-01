/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "launcher.h"
#include "logger.h"
#include "engine.h"
#include "exceptions.h"
#include "state/state_manager.h"
#include "state/tilemap_state.h"
#include "state/title_state.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

namespace impl {
namespace launcher {

  #define WINDOW_TITLE "SwampSurveyor"
  #define DEFAULT_SCALE false

  /**
   * Conversion to cfg from json
   * @param j the json to load
   * @param c the cfg to load into
   */
  void from_json(const json& j, launch_cfg_t& c) {
    j.at("window_width_p").get_to(c.window_width_p);
    j.at("window_height_p").get_to(c.window_height_p);
    j.at("window_scale").get_to(c.window_scale);
    j.at("tile_dim").get_to(c.tile_dim);
    j.at("debug").get_to(c.debug);
    j.at("title_image").get_to(c.title_image);
    j.at("caret_image").get_to(c.caret_image);
    j.at("level_cfgs").get_to(c.level_cfgs);
    j.at("major").get_to(c.major);
    j.at("minor").get_to(c.minor);
  }

  /**
   * Try to maximize the window scale
   * @param w pixel width
   * @param h pixel height
   * @return scale
   */
  [[nodiscard]] int try_rescale(int w, int h) {
    //get the width and height of the display
    SDL_DisplayMode disp_mode;
    SDL_GetCurrentDisplayMode(0, &disp_mode);
    return std::min(disp_mode.w / w, disp_mode.h / h);
  }

  /**
   * Launch the game from the launcher cfg
   * @param cfg_path the path to the configuration
   * @return success or failure
   */
  bool init_from_cfg(const launch_cfg_t& cfg) {

    /*
     * SDL initializations
     */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      logger::log_err("failed to init sdl_video: " +
                      std::string(SDL_GetError()));
      return false;
    }

    //probe the scale
    int win_scale = try_rescale(cfg.window_width_p, cfg.window_height_p);

    if (DEFAULT_SCALE) {
      win_scale = cfg.window_scale;
    }

    //get the scaled window dimensions
    int actual_width = cfg.window_width_p * win_scale;
    int actual_height = cfg.window_height_p * win_scale;
    std::string window_title = WINDOW_TITLE;

    if (cfg.debug) {
      window_title += " [debug]";
      logger::log_info("init window " + window_title + " - " + std::to_string(actual_width) +
                       "x" + std::to_string(actual_height));
    }

    //set scale quality
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"0")) {
      logger::log_err("failed to set sdl render quality");
      return false;
    }

    //create a window with the given dimensions
    SDL_Window *window = SDL_CreateWindow(
			    window_title.c_str(),
			    SDL_WINDOWPOS_UNDEFINED,
          SDL_WINDOWPOS_UNDEFINED,
			    actual_width, actual_height,
			    SDL_WINDOW_SHOWN);

    //initialize the renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                              SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
      logger::log_err("failed to init renderer: " + std::string(SDL_GetError()));
      return false;
    }

    //set the logical size of the renderer
    SDL_RenderSetLogicalSize(renderer,cfg.window_width_p,cfg.window_height_p);

    //clear
    SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);

    int img_init_flags = IMG_INIT_PNG;

    //setup png initialization
    if(!(IMG_Init(img_init_flags) & img_init_flags)) {
      logger::log_err("failed to init png load: " + std::string(IMG_GetError()));
      return false;
    }

    //setup ttf
    if (TTF_Init() < 0) {
      logger::log_err("failed to init ttf: " + std::string(TTF_GetError()));
      return false;
    }

    //disable the cursor
    SDL_ShowCursor(SDL_DISABLE);

    //create the camera
    SDL_Rect camera = {0,0,cfg.window_width_p,cfg.window_height_p};

    //return value after cleaning resources
    bool success = true;

    { //scope destroys state manager before renderer

      //the game state manager
      std::shared_ptr<state::state_manager_t> state_manager =
        std::make_shared<state::state_manager_t>(*renderer,
                                                 camera,
                                                 cfg.tile_dim,
                                                 cfg.font,
                                                 win_scale,
                                                 cfg.debug);

      //add the title state
      state_manager->add_state(std::make_unique<state::title_state_t>(cfg.window_width_p,
                                                                      cfg.window_height_p,
                                                                      cfg.title_image,
                                                                      cfg.caret_image,
                                                                      cfg.font,
                                                                      cfg.base_path,
                                                                      *renderer,
                                                                      *state_manager));

      //set the configuration paths in the state manager for future load
      state_manager->load_defer(cfg.level_cfgs, cfg.base_path, cfg.font);

      //title state not shown in debug mode
      if (cfg.debug) {
        //load first level right away
        state_manager->set_state(SWAMP_STATE);
      }

      //start entity update loop
      if (!engine::start_update_thread(state_manager)) {
        logger::log_err("failed to start update thread");
        success = false;
      } else {
        if (!engine::start_renderer(*renderer,state_manager,cfg.debug,cfg.font)) {
          logger::log_err("failed to start renderer");
          success = false;
        }
      }
    }

    //free resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    //quit subsystems
    IMG_Quit();
    SDL_Quit();

    return success;
  }

}}
