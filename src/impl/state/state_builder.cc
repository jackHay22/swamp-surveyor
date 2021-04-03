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
#include "../tilemap/procedural_tilemap.h"
#include "../tilemap/abstract_tilemap.h"
#include "../tilemap/tileset.h"
#include "../tilemap/transparent_block.h"
#include "../entity/entity.h"
#include "../entity/insects.h"
#include "../entity/entity_builder.h"
#include "../exceptions.h"
#include "../environment/renderable.h"
#include "../environment/environment_builder.h"
#include "../environment/environment.h"
#include "../items/item_builder.h"
#include "../items/item.h"
#include "../misc/map_fork.h"

namespace impl {
namespace state {

  //procedural map dimensions in tiles
  #define PROC_WIDTH_T 300
  #define PROC_HEIGHT_T 60

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
    //transparent blocks path
    std::string transparent_blocks_path = "";
    //map fork config path
    std::string map_fork_path = "";
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
    j.at("transparent_blocks_path").get_to(c.transparent_blocks_path);
    j.at("map_fork_path").get_to(c.map_fork_path);
  }

  /**
   * Load a state from a configuration
   * Throws a resource exception if load fails
   * @param  state_manager  the manager to load new states into
   * @param  path     the path to the level configuration
   * @param  renderer the renderer for the level
   * @param  camera   the starting level camera
   * @param  tile_dim the dimensions of tiles
   * @param  base_path resource directory base path
   * @param  font_path the path to the font to use
   * @param  idx_override put the state in a specific position
   */
  void load_tm_state(state::state_manager_t& state_manager,
                     const std::string& path,
                     SDL_Renderer& renderer,
                     SDL_Rect& camera,
                     int tile_dim,
                     const std::string& base_path,
                     const std::string& font_path,
                     int idx_override) {

    //load the file
    state_cfg_t cfg;

    try {
      std::ifstream in_stream(base_path + path);
      nlohmann::json config;

      in_stream >> config;
      cfg = config.get<state_cfg_t>();

    } catch (...) {
      throw exceptions::rsrc_exception_t(path);
    }

    //initialize the tileset from image
    std::shared_ptr<tilemap::tileset_t> tileset =
      std::make_shared<tilemap::tileset_t>(base_path + cfg.tileset_path,
                                           tile_dim,
                                           renderer);

    //add base path to map layer paths
    for (size_t i=0; i<cfg.map_layer_paths.size(); i++) {
      cfg.map_layer_paths.at(i) = base_path + cfg.map_layer_paths.at(i);
    }

    //the tilemap from layer paths
    std::shared_ptr<tilemap::tilemap_t> tilemap =
      std::make_shared<tilemap::tilemap_t>(cfg.map_layer_paths,
                                           tileset,
                                           cfg.entity_layer_idx,
                                           tile_dim,
                                           cfg.entity_layer_solid,
                                           cfg.entity_layer_water,
                                           cfg.bg_stationary);

    //entities list
    std::vector<std::shared_ptr<entity::entity_t>> entities;

    //load entities
    for (const std::string& epath : cfg.entity_cfg_paths) {
      entities.push_back(entity::load_entity(epath,renderer,tile_dim,base_path));
    }

    //load insects
    std::shared_ptr<entity::insects_t> insects =
      std::make_shared<entity::insects_t>(base_path + cfg.insect_cfg_path);

    //load renderable environmental elements
    std::vector<std::shared_ptr<environment::renderable_t>> env_renderable;
    environment::load_env_elems(env_renderable,
                                cfg.env_elems_path,
                                renderer,
                                base_path);

    //make environment from elements
    std::shared_ptr<environment::environment_t> env =
      std::make_shared<environment::environment_t>(env_renderable);

    //load items
    std::vector<std::shared_ptr<items::item_t>> level_items;
    items::load_items(level_items,
                      cfg.items_path,
                      renderer,
                      base_path);

    //load transparent blocks
    std::vector<std::shared_ptr<tilemap::transparent_block_t>> tblocks;
    tilemap::mk_transparent_blocks(tblocks,
                                   cfg.transparent_blocks_path,
                                   renderer,
                                   base_path);

    //load map forks
    std::vector<std::shared_ptr<misc::map_fork_t>> forks;
    misc::load_forks(forks,base_path + cfg.map_fork_path,font_path,renderer);

    //make the state and add it to the manager
    state_manager.add_state(std::make_unique<state::tilemap_state_t>(tilemap,
                                                                     entities,
                                                                     insects,
                                                                     env,
                                                                     level_items,
                                                                     tblocks,
                                                                     forks,
                                                                     cfg.player_idx,
                                                                     state_manager,
                                                                     camera,
                                                                     path),
                            //whether or not specified
                            idx_override);

  }

  /**
   * Load a new procedural tilemap state
   * @param  player   the previous player
   * @param  tile_dim the tile dimension
   * @param  camera   the camera to use
   * @param  renderer the renderer for creating textures
   * @param  manager  the state manager
   * @return          the new state
   */
  std::unique_ptr<state_t> load_procedural_state(std::shared_ptr<entity::player_t> player,
                                                 int tile_dim,
                                                 SDL_Rect& camera,
                                                 SDL_Renderer& renderer,
                                                 state_manager_t& manager) {

    std::vector<std::shared_ptr<entity::entity_t>> entities;
    entities.push_back(player);
    player->set_position(16,16);

    //create a procedural map
    std::shared_ptr<tilemap::abstract_tilemap_t> tilemap =
      std::make_shared<tilemap::procedural_tilemap_t>(
        tile_dim,
        PROC_WIDTH_T * tile_dim,
        PROC_HEIGHT_T * tile_dim,
        renderer
    );

    //TODO random insect generation
    std::shared_ptr<entity::insects_t> insects = std::make_shared<entity::insects_t>();
    std::shared_ptr<environment::environment_t> env = std::make_shared<environment::environment_t>();

    //empty
    std::vector<std::shared_ptr<items::item_t>> level_items;
    std::vector<std::shared_ptr<tilemap::transparent_block_t>> trans_blocks;
    std::vector<std::shared_ptr<misc::map_fork_t>> forks;

    std::string name = "generated";

    //create a new tilemap state
    return std::make_unique<state::tilemap_state_t>(
      //create a procedural tilemap
      tilemap,
      entities,
      insects,
      env,
      level_items,
      trans_blocks,
      forks,
      0,
      manager,
      camera,
      name,
      true //procedural
    );
  }
}}
