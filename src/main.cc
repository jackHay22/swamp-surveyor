/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include <string>
#include <unistd.h>
#include <json/nlohmann_json.h>
#include <fstream>
#include "impl/logger.h"
#include "impl/launcher.h"

/**
 * Setup the sdl window launcher
 * @param  debug whether debug mode enabled
 * @param  cfg_path the path to the configuration
 * @return       return value
 */
int setup(bool debug, const std::string& cfg_path) {

  //read from the configuration file
  std::ifstream in_stream(cfg_path);
  nlohmann::json config;

  if (debug) {
    impl::logger::log_info("debug mode enabled");
  }

  try {
    in_stream >> config;
    impl::launcher::launch_cfg_t cfg =
      config.get<impl::launcher::launch_cfg_t>();

    //override debug mode
    cfg.debug = debug;

    if (!impl::launcher::init_from_cfg(cfg)) {
      return EXIT_FAILURE;
    }

  } catch (...) {
    impl::logger::log_err("failed to load cfg as json: " + cfg_path);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

/**
 * Entrypoint
 * -- Command Line Arguments --
 * -d <debug>         | whether debug mode is enabled
 * -c <config_path>   | the path to the config file
 *
 * @param  argc number of args
 * @param  argv cmd line args
 * @return      exit status
 */
int main(int argc, char *argv[]) {

  int c;

  //whether debug mode enabled
  bool debug = false;
  std::string cfg_path = "resources/cfg.json";

  //get command line options (all values have defaults, none are required)
  while ((c = getopt(argc, argv, "dc:")) != -1) {
    if (c == 'd') {
      //parse server port
      debug = true;
    } else if (c == 'c') {
      cfg_path = std::string(optarg);
    }
  }

  return setup(debug, cfg_path);
}
