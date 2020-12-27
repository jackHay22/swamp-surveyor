/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "logger.h"
#include <iostream>

namespace impl {
namespace logger {

  /**
   * Log information
   * @param msg the message to log
   */
  void log_info(const std::string& msg) {
    std::cerr << "lvl=info,msg=\"" << msg << "\"" << std::endl;
  }

  /**
   * Log an error message
   * @param msg the message to log
   */
  void log_err(const std::string& msg) {
    std::cerr << "lvl=info,msg=\"" << msg << "\"" << std::endl;
  }
}}
