/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "entity_builder.h"
#include "../exceptions.h"
#include <fstream>
#include <json/nlohmann_json.h>
#include "player.h"

namespace impl {
namespace entity {
  const std::string PLAYER = "player";

  /**
   * Entity configuration values
   */
  struct entity_cfg_t {
    //the type of entity
    std::string entity_type = "undefined";
    //the width and height of the entity (for collisions)
    int width = 0;
    int height = 0;
    //player position x
    int x = 0;
    //player position y
    int y = 0;
    //the animation configurations for this entity
    std::vector<std::string> anim_paths;
  };

  /**
   * Conversion to cfg from json
   * @param j the json to load
   * @param c the cfg to load into
   */
  void from_json(const nlohmann::json& j, entity_cfg_t& c) {
    j.at("entity_type").get_to(c.entity_type);
    j.at("width").get_to(c.width);
    j.at("height").get_to(c.height);
    j.at("x").get_to(c.x);
    j.at("y").get_to(c.y);
    j.at("anim_paths").get_to(c.anim_paths);
  }

  /**
   * Load an entity from a cfg file
   * Throws rsrc_exception_t
   * @param  path the path to the cfg file
   * @param  renderer the renderer for loading textures
   * @param  tile_dim the dimension of tiles
   * @param  base_path the resource dir base path
   * @param  debug    whether debug mode enabled
   * @return      the entity
   */
  std::shared_ptr<entity_t> load_entity(const std::string& path,
                                        SDL_Renderer& renderer,
                                        int tile_dim,
                                        const std::string& base_path,
                                        bool debug) {

    try {
      //load the config file
      std::ifstream in_stream(base_path + path);
      nlohmann::json config;

      in_stream >> config;
      entity_cfg_t cfg = config.get<entity_cfg_t>();

      //determine the player type
      if (cfg.entity_type == PLAYER) {
        //load the player
        return std::make_shared<entity::player_t>(cfg.x,cfg.y, //136,82 for level 1
                                                  cfg.width,
                                                  cfg.height,
                                                  cfg.anim_paths,
                                                  renderer,
                                                  tile_dim,
                                                  base_path,
                                                  debug);
      }

      //unknown entity type
      throw exceptions::rsrc_exception_t("unknown entity type: " + cfg.entity_type);

    } catch (exceptions::rsrc_exception_t& e) {
      //rethrow a resource exception
      throw e;

    } catch (...) {
      throw exceptions::rsrc_exception_t("failed to load as json: " + base_path + path);
    }
  }
}}
