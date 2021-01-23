/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "environment_builder.h"
#include "../exceptions.h"
#include <json/nlohmann_json.h>
#include <fstream>
#include "chemical_foam.h"
#include "dead_tree.h"
#include "chemical_seep.h"
#include "door.h"
#include "pushable.h"

namespace impl {
namespace environment {

  typedef nlohmann::json json;

  #define CHEMICAL_FOAM_TYPE "chemical_foam"
  #define DEAD_TREE_TYPE "dead_tree"
  #define CHEMICAL_SEEP_TYPE "chemical_seep"
  #define DOOR_TYPE "door"
  #define PUSHABLE_TYPE "pushable"

  /**
   * Configuration options
   */
  struct env_set_cfg {
    int x;
    int y;
    int w;
    int h;
    int range;
    float density;
    std::string animation_path;
    int animation_frames;
    std::string type;
  };

  /**
   * Conversion to cfg from json
   * @param j the json to load
   * @param c the cfg to load into
   */
  void from_json(const json& j, env_set_cfg& c) {
    j.at("x").get_to(c.x);
    j.at("y").get_to(c.y);
    j.at("w").get_to(c.w);
    j.at("h").get_to(c.h);
    j.at("range").get_to(c.range);
    j.at("density").get_to(c.density);
    j.at("animation_path").get_to(c.animation_path);
    j.at("animation_frames").get_to(c.animation_frames);
    j.at("type").get_to(c.type);
  }

  /**
   * Load renderable environmental elements
   * @param elems    a vector of loaded elements set by the call
   * @param cfg_path the path to the environment configuration
   * @param renderer the sdl renderer
   * @param base_path the resource folder base path
   * @param debug    whether debug mode is enabled
   */
  void load_env_elems(std::vector<std::shared_ptr<environment::renderable_t>>& elems,
                      const std::string& cfg_path,
                      SDL_Renderer& renderer,
                      const std::string& base_path,
                      bool debug) {
    nlohmann::json config;

    try {
      std::ifstream in_stream(base_path + cfg_path);
      in_stream >> config;

      //load each env element
      for (json& env : config) {
        //load the configuration
        env_set_cfg cfg = env.get<env_set_cfg>();

        //check the type
        if (cfg.type == CHEMICAL_FOAM_TYPE) {
          //add the chemical foam element to the environment
          elems.push_back(std::make_shared<environment::chemical_foam_t>(cfg.x,
                                                                         cfg.y,
                                                                         cfg.w,
                                                                         cfg.h,
                                                                         cfg.density,
                                                                         debug));
        } else if (cfg.type == DEAD_TREE_TYPE) {
          //add a dead tree to the environment
          elems.push_back(std::make_shared<environment::dead_tree_t>(cfg.x,
                                                                     cfg.y,
                                                                     cfg.w,
                                                                     cfg.h,
                                                                     base_path + cfg.animation_path,
                                                                     renderer,
                                                                     cfg.animation_frames,
                                                                     debug));
        } else if (cfg.type == CHEMICAL_SEEP_TYPE) {
          //add a chemical seep to the environment
          elems.push_back(std::make_shared<environment::chemical_seep_t>(cfg.x,
                                                                         cfg.y,
                                                                         cfg.w,
                                                                         cfg.h,
                                                                         debug));
        } else if (cfg.type == DOOR_TYPE) {
          //add a door to the environment
          elems.push_back(std::make_shared<environment::door_t>(cfg.x,
                                                                cfg.y,
                                                                cfg.w,
                                                                cfg.h,
                                                                base_path + cfg.animation_path,
                                                                renderer,
                                                                cfg.animation_frames,
                                                                debug));
        } else if (cfg.type == PUSHABLE_TYPE) {
          //add the pushable element
          elems.push_back(std::make_shared<environment::pushable_t>(cfg.x,
                                                                    cfg.y,
                                                                    cfg.w,
                                                                    cfg.h,
                                                                    cfg.range,
                                                                    cfg.y + (cfg.h - 2),
                                                                    base_path + cfg.animation_path,
                                                                    renderer,
                                                                    debug));
        }
      }

    } catch (...) {
      throw exceptions::rsrc_exception_t(base_path + cfg_path);
    }
  }
}}
