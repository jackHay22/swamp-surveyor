/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_LOGGER_H
#define _IO_JACKHAY_SWAMP_LOGGER_H

#include <string>

namespace impl {
namespace logger {

  /**
   * Log information
   * @param msg the message to log
   */
  void log_info(const std::string& msg);

  /**
   * Log an error message
   * @param msg the message to log
   */
  void log_err(const std::string& msg);
}}

#endif /*_IO_JACKHAY_SWAMP_LOGGER_H*/
