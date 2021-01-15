/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_EXCEPTIONS_H
#define _IO_JACKHAY_SWAMP_EXCEPTIONS_H

#include <exception>
#include <string>

namespace impl {
namespace exceptions {

  /**
   * Exception thrown when a resource can't be loaded
   */
  struct rsrc_exception_t : public std::exception {
  private:
    std::string detail;
    std::string path;
  public:
    //constructor with path
    rsrc_exception_t(const std::string& path)
      : path(path), detail("occured loading ") {}
    rsrc_exception_t(const std::string& path,
                     const std::string& detail)
      : path(path), detail(detail) {}
    rsrc_exception_t(const rsrc_exception_t& other)
      : path(other.path), detail(other.detail) {}
    rsrc_exception_t& operator=(const rsrc_exception_t&) = delete;

    /**
     * Get the error message
     * @return error message
     */
    const std::string trace() const throw () {
      return detail + ": " + path;
    }
  };
}}

#endif /*_IO_JACKHAY_SWAMP_EXCEPTIONS_H*/
