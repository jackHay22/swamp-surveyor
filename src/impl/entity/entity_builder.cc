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
    //player position, bounds
    int pos_x = 0;
    int pos_y = 0;
    int width = 0;
    int height = 0;

    //the animation configurations for this entity
    std::vector<std::string> anim_paths;
  };

  /**
   * Conversion from cfg to json
   * @param j the json
   * @param c the cfg to convert
   */
  void to_json(nlohmann::json& j, const entity_cfg_t& c) {
    j = nlohmann::json{
             {"entity_type", c.entity_type},
             {"pos_x", c.pos_x},
             {"pos_y", c.pos_y},
             {"width", c.width},
             {"height", c.height},
             {"anim_paths", c.anim_paths}
           };
  }

  /**
   * Conversion to cfg from json
   * @param j the json to load
   * @param c the cfg to load into
   */
  void from_json(const nlohmann::json& j, entity_cfg_t& c) {
    j.at("entity_type").get_to(c.entity_type);
    j.at("pos_x").get_to(c.pos_x);
    j.at("pos_y").get_to(c.pos_y);
    j.at("width").get_to(c.width);
    j.at("height").get_to(c.height);
    j.at("anim_paths").get_to(c.anim_paths);
  }

  /**
   * Load an entity from a cfg file
   * Throws rsrc_exception_t
   * @param  path the path to the cfg file
   * @param  renderer the renderer for loading textures
   * @return      the entity
   */
  std::shared_ptr<entity_t> load_entity(const std::string& path,
                                        SDL_Renderer& renderer) {
    //load the config file
    std::ifstream in_stream(path);
    nlohmann::json config;

    try {
      in_stream >> config;
      entity_cfg_t cfg = config.get<entity_cfg_t>();

      //determine the player type
      if (cfg.entity_type == PLAYER) {
        //load the player
        return std::make_shared<entity::player_t>(cfg.pos_x,
                                                  cfg.pos_y,
                                                  cfg.width,
                                                  cfg.height,
                                                  cfg.anim_paths,
                                                  renderer);
      }

      //unknown entity type
      throw exceptions::rsrc_exception_t("unknown entity type: " + cfg.entity_type);

    } catch (exceptions::rsrc_exception_t& e) {
      //rethrow a resource exception
      throw e;
      
    } catch (...) {
      throw exceptions::rsrc_exception_t("failed to load as json: " + path);
    }
  }
}}
