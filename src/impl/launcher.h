/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_LAUNCHER_H
#define _IO_JACKHAY_SWAMP_LAUNCHER_H

#include <vector>
#include <string>
#include <json/nlohmann_json.h>

namespace impl {
namespace launcher {

  typedef nlohmann::json json;

  typedef struct launch_cfg_t {
    //graphical parameters
    int window_width_p = 256;
    int window_height_p = 160;
    float window_scale = 5;
    int tile_dim = 8;
    //default debug mode
    bool debug = false;
    //the title screen image
    std::string title_image = "title_splash.png";
    //the caret image for menu selections
    std::string caret_image = "caret.png";
    //the font for menu options
    std::string font = "/usr/share/fonts/noto/NotoSans-Light.ttf";
    //the configurations for each level
    std::vector<std::string> level_cfgs = {};
    //the base directory path
    std::string base_path = "resources/";
  } launch_cfg_t;

  /**
   * Conversion from cfg to json
   * @param j the json
   * @param c the cfg to convert
   */
  void to_json(json& j, const launch_cfg_t& c);

  /**
   * Conversion to cfg from json
   * @param j the json to load
   * @param c the cfg to load into
   */
  void from_json(const json& j, launch_cfg_t& c);

  /**
   * Launch the game from the launcher cfg
   * @param cfg the configuration
   * @return success or failure
   */
  bool init_from_cfg(const launch_cfg_t& cfg);

}}

#endif /*_IO_JACKHAY_SWAMP_LAUNCHER_H*/
