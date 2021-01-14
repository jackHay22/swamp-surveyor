/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "launcher.h"
#include "logger.h"
#include "engine.h"
#include "exceptions.h"
#include "tilemap/tilemap.h"
#include "tilemap/tileset.h"
#include "entity/entity.h"
#include "entity/entity_builder.h"
#include "state/state_manager.h"
#include "state/tilemap_state.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace impl {
namespace launcher {

  /**
   * Conversion from cfg to json
   * @param j the json
   * @param c the cfg to convert
   */
  void to_json(json& j, const launch_cfg_t& c) {
    j = json{{"window_width_p", c.window_width_p},
             {"window_height_p", c.window_height_p},
             {"window_scale", c.window_scale},
             {"tile_dim", c.tile_dim},
             {"debug", c.debug},
             {"tileset_path", c.tileset_path},
             {"map_layer_paths", c.map_layer_paths},
             {"entity_layer_idx", c.entity_layer_idx},
             {"entity_layer_solid", c.entity_layer_solid},
             {"entity_layer_water", c.entity_layer_water},
             {"entity_cfg_paths", c.entity_cfg_paths},
             {"player_idx", c.player_idx}
           };
  }

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
    j.at("tileset_path").get_to(c.tileset_path);
    j.at("map_layer_paths").get_to(c.map_layer_paths);
    j.at("entity_layer_idx").get_to(c.entity_layer_idx);
    j.at("entity_layer_solid").get_to(c.entity_layer_solid);
    j.at("entity_layer_water").get_to(c.entity_layer_water);
    j.at("entity_cfg_paths").get_to(c.entity_cfg_paths);
    j.at("player_idx").get_to(c.player_idx);
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

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
      logger::log_err("failed to init sdl_image: " +
                     std::string(IMG_GetError()));
      return false;
    }

    //get the scaled window dimensions
    int actual_width = cfg.window_width_p * cfg.window_scale;
    int actual_height = cfg.window_height_p * cfg.window_scale;
    std::string window_title = "swamp";

    if (cfg.debug) {
      window_title += " [debug]";
      logger::log_info("init window " + window_title + " - " + std::to_string(actual_width) +
                       "x" + std::to_string(actual_height));
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

    //set scale quality
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,0);

    //set the logical size of the renderer
    SDL_RenderSetLogicalSize(renderer,cfg.window_width_p,cfg.window_height_p);

    //clear
    SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);

    //setup png initialization
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
      logger::log_err("failed to init png load: " + std::string(IMG_GetError()));
      return false;
    }

    //create the camera
    SDL_Rect camera = {0,0,actual_width,actual_height};

    //the game state manager
    std::shared_ptr<state::state_manager_t> state_manager;

    try {
      //initialize the tileset from image
      std::shared_ptr<tilemap::tileset_t> tileset =
        std::make_shared<tilemap::tileset_t>(cfg.tileset_path,
                                             cfg.tile_dim,
                                             *renderer);

      //the tilemap from layer paths
      std::shared_ptr<tilemap::tilemap_t> tilemap =
        std::make_shared<tilemap::tilemap_t>(cfg.map_layer_paths,
                                             tileset,
                                             cfg.entity_layer_idx,
                                             cfg.entity_layer_solid,
                                             cfg.entity_layer_water);

      //entities list
      std::vector<std::shared_ptr<entity::entity_t>> entities;

      //load entities, add to state manager
      for (const std::string& epath : cfg.entity_cfg_paths) {
        entities.push_back(entity::load_entity(epath,*renderer));
      }

      //init state manager
      state_manager = std::make_shared<state::state_manager_t>();
      //add tilemap state
      state_manager->add_state(std::make_unique<state::tilemap_state_t>(tilemap,
                                                                        entities,
                                                                        cfg.player_idx,
                                                                        camera));

    } catch (exceptions::rsrc_exception_t& e) {
      logger::log_err(e.trace());
      return false;
    } catch (...) {
      logger::log_err("unknown exception when loading resoures");
      return false;
    }

    bool success = true;

    //start entity update loop
    if (!engine::start_update_thread(state_manager)) {
      logger::log_err("failed to start update thread");
      success = false;
    } else {
      if (!engine::start_renderer(*renderer,camera,state_manager)) {
        logger::log_err("failed to start renderer");
        success = false;
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
