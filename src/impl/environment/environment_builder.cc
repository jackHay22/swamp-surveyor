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
#include "crows.h"
#include "procedural_trees.h"

namespace impl {
namespace environment {

  typedef nlohmann::json json;

  #define CHEMICAL_FOAM_TYPE "chemical_foam"
  #define DEAD_TREE_TYPE "dead_tree"
  #define CHEMICAL_SEEP_TYPE "chemical_seep"
  #define DOOR_TYPE "door"
  #define PUSHABLE_TYPE "pushable"
  #define CROW_TYPE "crows"
  #define PROC_TREES "procedural_trees"

  /**
   * Check if json has some int by key
   * @param  j   the json
   * @param  key the key
   * @return     the number or -1 if not found
   */
  int get_int(const json j, const std::string& key) {
    //look for the key
    if (j.count(key) && j[key].is_number()) {
      return j[key].get<int>();
    }
    return -1;
  }

  /**
   * Configuration options
   */
  struct env_set_cfg {
    int x;
    int y;
    int w;
    int h;
    float density;
    std::string animation_path;
    int animation_frames;
    std::string type;

    //optional: interactive bounds
    int interact_x = 0;
    int interact_y = 0;
    int interact_w = 0;
    int interact_h = 0;

    //optional: range value
    int range = 0;
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
    j.at("density").get_to(c.density);
    j.at("animation_path").get_to(c.animation_path);
    j.at("animation_frames").get_to(c.animation_frames);
    j.at("type").get_to(c.type);

    //get optional elements
    c.interact_x = get_int(j,"interact_x");
    c.interact_y = get_int(j,"interact_y");
    c.interact_w = get_int(j,"interact_w");
    c.interact_h = get_int(j,"interact_h");
    c.range = get_int(j,"range");
  }

  /**
   * Load renderable environmental elements
   * @param elems    a vector of loaded elements set by the call
   * @param cfg_path the path to the environment configuration
   * @param renderer the sdl renderer
   * @param base_path the resource folder base path
   */
  void load_env_elems(std::vector<std::shared_ptr<environment::renderable_t>>& elems,
                      const std::string& cfg_path,
                      SDL_Renderer& renderer,
                      const std::string& base_path) {
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
                                                                         cfg.density));
        } else if (cfg.type == DEAD_TREE_TYPE) {
          //add a dead tree to the environment
          elems.push_back(std::make_shared<environment::dead_tree_t>(cfg.x,
                                                                     cfg.y,
                                                                     cfg.w,
                                                                     cfg.h,
                                                                     base_path + cfg.animation_path,
                                                                     renderer,
                                                                     cfg.animation_frames));
        } else if (cfg.type == CHEMICAL_SEEP_TYPE) {
          //add a chemical seep to the environment
          elems.push_back(std::make_shared<environment::chemical_seep_t>(cfg.x,
                                                                         cfg.y,
                                                                         cfg.w,
                                                                         cfg.h));
        } else if (cfg.type == DOOR_TYPE) {
          //add a door to the environment
          elems.push_back(std::make_shared<environment::door_t>(cfg.x,
                                                                cfg.y,
                                                                cfg.w,
                                                                cfg.h,
                                                                base_path + cfg.animation_path,
                                                                renderer,
                                                                cfg.animation_frames));
        } else if (cfg.type == PUSHABLE_TYPE) {
          SDL_Rect interact_bounds = {cfg.interact_x, cfg.interact_y,
                                      cfg.interact_w, cfg.interact_h};
          SDL_Rect solid_bounds = {cfg.x, cfg.y, cfg.w, cfg.h};
          //add the pushable element
          elems.push_back(std::make_shared<environment::pushable_t>(interact_bounds,
                                                                    solid_bounds,
                                                                    cfg.range,
                                                                    base_path + cfg.animation_path,
                                                                    renderer));
        } else if (cfg.type == CROW_TYPE) {
          elems.push_back(std::make_shared<environment::crows_t>((int)cfg.density,
                                                                 cfg.w,
                                                                 cfg.animation_path,
                                                                 base_path,
                                                                 renderer));
        } else if (cfg.type == PROC_TREES) {
          SDL_Rect region = {cfg.x,cfg.y,cfg.w,cfg.h};
          elems.push_back(std::make_shared<environment::procedural_trees_t>(region,
                                                                            renderer,
                                                                            cfg.animation_frames));
        }
      }

    } catch (...) {
      throw exceptions::rsrc_exception_t(base_path + cfg_path);
    }
  }
}}
