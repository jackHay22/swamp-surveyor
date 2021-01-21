/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "map_fork.h"
#include "../utils.h"
#include "../exceptions.h"
#include <json/nlohmann_json.h>
#include <fstream>

namespace impl {
namespace misc {

  #define FONT_SIZE 12
  #define FORK_RADIUS 24

  typedef nlohmann::json json;

  //the configuration of each block
  struct fork_cfg {
    int x;
    int y;
    std::string target_text;
    std::string target_name;
    std::string font_path;
  };

  /**
   * Conversion to cfg from json
   * @param j the json to load
   * @param c the cfg to load into
   */
  void from_json(const json& j, fork_cfg& c) {
    j.at("x").get_to(c.x);
    j.at("y").get_to(c.y);
    j.at("target_text").get_to(c.target_text);
    j.at("target_name").get_to(c.target_name);
    j.at("font_path").get_to(c.font_path);
  }

  /**
   * Load map forks
   * @param forks    forks loaded by the call
   * @param path     path to the fork configuration file
   * @param renderer the sdl renderer for loading textures
   */
  void load_forks(std::vector<std::shared_ptr<map_fork_t>>& forks,
                  const std::string& path,
                  SDL_Renderer& renderer) {
    try {
      nlohmann::json config;
      std::ifstream in_stream(path);
      in_stream >> config;

      //load each env element
      for (json& env : config) {
        //load the configuration
        fork_cfg cfg = env.get<fork_cfg>();

        //add a new fork from cfg
        forks.push_back(std::make_shared<map_fork_t>(cfg.x,
                                                     cfg.y,
                                                     cfg.target_text,
                                                     cfg.target_name,
                                                     cfg.font_path,
                                                     renderer));
      }

    } catch (...) {
      throw exceptions::rsrc_exception_t(path);
    }
  }

  /**
   * Constructor
   * @param x           fork position x
   * @param y           fork position y
   * @param target_text the name to display to player
   * @param target_name the name of the target state
   * @param font_path   path to the font to load
   * @param renderer    renderer for loading the font
   */
  map_fork_t::map_fork_t(int x, int y,
                         const std::string& target_text,
                         const std::string& target_name,
                         const std::string& font_path,
                         SDL_Renderer& renderer)
    : x(x), y(y), display(false) {
    //load text to texture
    texture = utils::load_font(target_text,
                               font_path,
                               renderer,
                               FONT_SIZE,
                               texture_w,
                               texture_h,
                               {255,255,255});

    //determine the destination based on a name
    if (target_name == "swamp") {
      dest = state::SWAMP;
    } else if (target_name == "tracks") {
      dest = state::TRACKS;
    } else {
      throw exceptions::rsrc_exception_t("no such state: " + target_name);
    }
  }

  /**
   * Free texture
   */
  map_fork_t::~map_fork_t() {
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }

  /**
   * Check if a bounding box collides with this fork location
   * @param  bounds the bounds to check
   * @return        whether the bounds collide
   */
  bool map_fork_t::is_collided(const SDL_Rect& bounds) const {
    return ((x < (bounds.x + bounds.w)) &&
            ((x + FORK_RADIUS) > bounds.x) &&
            (y < (bounds.y + bounds.h)) &&
            ((y + FORK_RADIUS) > bounds.y));
  }

  /**
   * Update the fork message
   * @param player the position of the player
   */
  void map_fork_t::update(const SDL_Rect& player) {
    this->display = this->is_collided(player);
  }

  /**
   * Render the message about this fork
   * @param renderer the renderer to use
   * @param camera   the camera
   */
  void map_fork_t::render(SDL_Renderer& renderer,
                          const SDL_Rect& camera) const {

    if (display) {
      //render the message
      SDL_Rect sample_bounds = {0,0,texture_w,texture_h};
      SDL_Rect text_bounds = {(camera.w / 2) - (texture_w / 2),
                              camera.h - (2 * FONT_SIZE),
                              texture_w,texture_h};

      //render the start texture
      SDL_RenderCopy(&renderer,
                     texture,
                     &sample_bounds,
                     &text_bounds);
    }
  }
}}
