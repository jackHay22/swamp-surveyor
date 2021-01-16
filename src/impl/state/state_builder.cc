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
#include "../tilemap/tilemap.h"
#include "../tilemap/tileset.h"
#include "../entity/entity.h"
#include "../entity/insects.h"
#include "../entity/entity_builder.h"
#include "../exceptions.h"

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
    //the insect swarm cfg file
    std::string insect_cfg_path = "";
    //index of player in entity cfg list
    int player_idx = 0;
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
    j.at("player_idx").get_to(c.player_idx);
  }

  /**
   * Load a state from a configuration
   * Throws a resource exception if load fails
   * @param  path     the path to the level configuration
   * @param  renderer the renderer for the level
   * @param  camera   the starting level camera
   * @param  tile_dim the dimensions of tiles
   * @param  debug    whether debug mode enabled
   * @return          the loaded level
   */
  std::unique_ptr<state_t> load_tm_state(const std::string& path,
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
                                           debug);

    //entities list
    std::vector<std::shared_ptr<entity::entity_t>> entities;

    //load entities
    for (const std::string& epath : cfg.entity_cfg_paths) {
      entities.push_back(entity::load_entity(epath,renderer,tile_dim,debug));
    }

    //load insects
    std::shared_ptr<entity::insects_t> insects =
      std::make_shared<entity::insects_t>(cfg.insect_cfg_path,renderer);

    //make the state
    return std::make_unique<state::tilemap_state_t>(tilemap,
                                                    entities,
                                                    insects,
                                                    cfg.player_idx,
                                                    camera);

  }
}}