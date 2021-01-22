/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include <string>
#include <unistd.h>
#include <json/nlohmann_json.h>
#include <fstream>
#include "impl/exceptions.h"
#include "impl/logger.h"
#include "impl/launcher.h"

/**
 * Setup the sdl window launcher
 * @param  debug whether debug mode enabled
 * @param  base_path the path to the configuration
 * @param  font_path the path to the font to use
 * @param  cfg_name  the name of the cfg file
 * @return       return value
 */
int setup(bool debug,
          const std::string& base_path,
          const std::string& font_path,
          const std::string& cfg_name) {

  //read from the configuration file
  std::ifstream in_stream(base_path + cfg_name);
  nlohmann::json config;

  if (debug) {
    impl::logger::log_info("debug mode enabled");
  }

  try {
    in_stream >> config;
    impl::launcher::launch_cfg_t cfg =
      config.get<impl::launcher::launch_cfg_t>();

    if (debug) {
      //override debug mode
      cfg.debug = debug;
    }

    //set base directory and font paths
    cfg.base_path = base_path;
    cfg.font = font_path;

    if (!impl::launcher::init_from_cfg(cfg)) {
      return EXIT_FAILURE;
    }

  } catch (impl::exceptions::rsrc_exception_t& e) {
    impl::logger::log_err("failed to load cfg: " + e.trace());
    return EXIT_FAILURE;

  }

  return EXIT_SUCCESS;
}

/**
 * Entrypoint
 * -- Command Line Arguments --
 * -d <debug>         | whether debug mode is enabled
 * -c <config_path>   | the path to the config file
 * -b <base_path>     | directory where cfg is
 *
 * @param  argc number of args
 * @param  argv cmd line args
 * @return      exit status
 */
int main(int argc, char *argv[]) {

  int c;

  //whether debug mode enabled
  bool debug = false;

  //the name of the cfg file
  std::string cfg_name = "cfg.json";

  #ifdef BUILD__MACOS__
  std::string font = "/System/Library/Fonts/SFCompactDisplay-Thin.otf";
  std::string base_path = "/Library/Application Support/io.jackhay/swamp_surveyor/";
  #else
  std::string font = "/usr/share/fonts/noto/NotoSans-Light.ttf";
  std::string base_path = "resources/";
  #endif

  //get command line options (all values have defaults, none are required)
  while ((c = getopt(argc, argv, "dc:b:")) != -1) {
    if (c == 'd') {
      //parse server port
      debug = true;
    } else if (c == 'c') {
      cfg_name = std::string(optarg);
    } else if (c == 'b') {
      base_path = std::string(optarg);
    }
  }

  //load resources and launch
  return setup(debug,base_path,font,cfg_name);
}
