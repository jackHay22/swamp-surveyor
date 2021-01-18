/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ACTIONS_PUSH_H
#define _IO_JACKHAY_SWAMP_ACTIONS_PUSH_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "action.h"

namespace impl {
namespace entity {
namespace actions {

  /**
   * Pushing something (currently a noop)
   */
  struct push_t : public action_t {
  public:
    push_t() : action_t() {}
    push_t(const push_t&) = delete;
    push_t& operator=(const push_t&) = delete;

  };
}}}

#endif /*_IO_JACKHAY_SWAMP_ACTIONS_PUSH_H*/
