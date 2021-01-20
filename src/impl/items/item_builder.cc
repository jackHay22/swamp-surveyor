/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "item_builder.h"
#include "../exceptions.h"
#include <json/nlohmann_json.h>
#include <fstream>
#include "chemical_label.h"
#include "sprayer.h"

namespace impl {
namespace items {

  typedef nlohmann::json json;

  #define CHEMICAL_LABEL_TYPE "chemical_label"
  #define FOAM_SPRAYER_TYPE "foam_sprayer"

  /**
   * Configuration options
   */
  struct item_cfg {
    int x;
    int y;
    std::string texture_path;
    std::string type;
  };

  /**
   * Conversion to cfg from json
   * @param j the json to load
   * @param c the cfg to load into
   */
  void from_json(const json& j, item_cfg& c) {
    j.at("x").get_to(c.x);
    j.at("y").get_to(c.y);
    j.at("texture_path").get_to(c.texture_path);
    j.at("type").get_to(c.type);
  }

  /**
   * Load items from cfg
   * @param items    a vector of loaded items set by the call
   * @param cfg_path the path to the items configuration
   * @param renderer the sdl renderer
   * @param debug    whether debug mode is enabled
   */
  void load_items(std::vector<std::shared_ptr<item_t>>& items,
                  const std::string& cfg_path,
                  SDL_Renderer& renderer,
                  bool debug) {
    nlohmann::json config;

    try {
      std::ifstream in_stream(cfg_path);
      in_stream >> config;

      //load each env element
      for (json& env : config) {
        //load the configuration
        item_cfg cfg = env.get<item_cfg>();

        //check the type of the item and load
        if (cfg.type == CHEMICAL_LABEL_TYPE) {
          items.push_back(std::make_shared<chemical_label_t>(cfg.x,
                                                             cfg.y,
                                                             cfg.texture_path,
                                                             renderer,
                                                             debug));
        } else if (cfg.type == FOAM_SPRAYER_TYPE) {
          items.push_back(std::make_shared<sprayer_t>(cfg.x,
                                                      cfg.y,
                                                      cfg.texture_path,
                                                      renderer,
                                                      debug));
        }
      }

    } catch (...) {
      throw exceptions::rsrc_exception_t(cfg_path);
    }
  }
}}
