/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "state_builder.h"
#include "tilemap_state.h"
#include "../logger.h"
#include <json/nlohmann_json.h>
#include <fstream>
#include <memory>
#include "../tilemap/tilemap.h"
#include "../tilemap/tileset.h"
#include "../entity/entity.h"
#include "../entity/insects.h"
#include "../entity/entity_builder.h"
#include "../exceptions.h"
#include "../environment/renderable.h"
#include "../environment/environment_builder.h"
#include "../items/item_builder.h"
#include "../items/item.h"

namespace impl {
namespace state {

  typedef nlohmann::json json;

  //the configuration of each tilemap state
  struct state_cfg_t {
    //tileset path
    std::string tileset_path = "";
    //resource information
    std::vector<std::string> map_layer_paths = {};
    //index of entity tilee layer map list
    int entity_layer_idx = 2;
    //indices of tiles that are solid ground
    std::vector<int> entity_layer_solid = {};
    //indices of tiles that are liquid (water)
    std::vector<int> entity_layer_water = {};
    //entity files
    std::vector<std::string> entity_cfg_paths = {};
    //the path to the environment configuration for this level
    std::string env_elems_path = "";
    //whether the background layer is stationary
    bool bg_stationary = false;
    //the insect swarm cfg file
    std::string insect_cfg_path = "";
    //index of player in entity cfg list
    int player_idx = 0;
    //the path to the items configuration file
    std::string items_path;
  };

  /**
   * Conversion to cfg from json
   * @param j the json to load
   * @param c the cfg to load into
   */
  void from_json(const json& j, state_cfg_t& c) {
    j.at("tileset_path").get_to(c.tileset_path);
    j.at("map_layer_paths").get_to(c.map_layer_paths);
    j.at("entity_layer_idx").get_to(c.entity_layer_idx);
    j.at("entity_layer_solid").get_to(c.entity_layer_solid);
    j.at("entity_layer_water").get_to(c.entity_layer_water);
    j.at("entity_cfg_paths").get_to(c.entity_cfg_paths);
    j.at("insect_cfg_path").get_to(c.insect_cfg_path);
    j.at("env_elems_path").get_to(c.env_elems_path);
    j.at("player_idx").get_to(c.player_idx);
    j.at("bg_stationary").get_to(c.bg_stationary);
    j.at("items_path").get_to(c.items_path);
  }

  /**
   * Load a state from a configuration
   * Throws a resource exception if load fails
   * @param  state_manager  the manager to load new states into
   * @param  path     the path to the level configuration
   * @param  renderer the renderer for the level
   * @param  camera   the starting level camera
   * @param  tile_dim the dimensions of tiles
   * @param  debug    whether debug mode enabled
   */
  void load_tm_state(state::state_manager_t& state_manager,
                     const std::string& path,
                     SDL_Renderer& renderer,
                     SDL_Rect& camera,
                     int tile_dim,
                     bool debug) {

    //load the file
    state_cfg_t cfg;

    try {
      std::ifstream in_stream(path);
      nlohmann::json config;

      in_stream >> config;
      cfg = config.get<state_cfg_t>();

    } catch (...) {
      throw exceptions::rsrc_exception_t(path);
    }

    //initialize the tileset from image
    std::shared_ptr<tilemap::tileset_t> tileset =
      std::make_shared<tilemap::tileset_t>(cfg.tileset_path,
                                           tile_dim,
                                           renderer,
                                           debug);

    //the tilemap from layer paths
    std::shared_ptr<tilemap::tilemap_t> tilemap =
      std::make_shared<tilemap::tilemap_t>(cfg.map_layer_paths,
                                           tileset,
                                           cfg.entity_layer_idx,
                                           tile_dim,
                                           cfg.entity_layer_solid,
                                           cfg.entity_layer_water,
                                           cfg.bg_stationary,
                                           debug);

    //entities list
    std::vector<std::shared_ptr<entity::entity_t>> entities;

    //load entities
    for (const std::string& epath : cfg.entity_cfg_paths) {
      entities.push_back(entity::load_entity(epath,renderer,tile_dim,debug));
    }

    //load insects
    std::shared_ptr<entity::insects_t> insects =
      std::make_shared<entity::insects_t>(cfg.insect_cfg_path);

    //load renderable environmental elements
    std::vector<std::shared_ptr<environment::renderable_t>> env_renderable;
    environment::load_env_elems(env_renderable,
                                cfg.env_elems_path,
                                renderer,
                                debug);

    //load items
    std::vector<std::shared_ptr<items::item_t>> level_items;
    items::load_items(level_items,
                      cfg.items_path,
                      renderer, debug);

    //make the state and add it to the manager
    state_manager.add_state(std::make_unique<state::tilemap_state_t>(tilemap,
                                                                     entities,
                                                                     insects,
                                                                     env_renderable,
                                                                     level_items,
                                                                     cfg.player_idx,
                                                                     state_manager,
                                                                     camera));

  }
}}
