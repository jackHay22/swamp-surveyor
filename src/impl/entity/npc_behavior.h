/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_NPC_BEHAVIOR_H
#define _IO_JACKHAY_SWAMP_NPC_BEHAVIOR_H

#include "entity.h"
#include <vector>
#include "../tilemap/tilemap.h"

namespace impl {
namespace entity {
namespace npc_behavior {

  /**
   * The behavior of a surveyor working with the main player
   * @param pos the positions of all other entities
   * @param map the tilemap
   * @param x    self position x
   * @param y    self position y
   * @param state the state set based on decision
   * @param facing_left set by the call (updates entity facing direction)
   */
  void surveyor(const std::vector<entity_pos_t>& pos,
                const tilemap::tilemap_t& map,
                int x, int y,
                entity_state& state,
                bool& facing_left);

}}}

#endif /*_IO_JACKHAY_SWAMP_NPC_BEHAVIOR_H*/
