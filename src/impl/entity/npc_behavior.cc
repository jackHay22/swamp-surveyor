/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "npc_behavior.h"
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <cmath>

namespace impl {
namespace entity {
namespace npc_behavior {

  // the distance another surveyor keeps between it and the player
  #define SURVEYOR_FOLLOW_DIST 60

  /**
   * Get the distance between two points
   * @return    distance
   */
  int distance(int x1, int y1, int x2, int y2) {
    return std::sqrt(
            pow(std::abs(x1 - x2),2) +
            pow(std::abs(y1 - y2),2));
  }

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
                const tilemap::abstract_tilemap_t& map,
                int x, int y,
                entity_state& state,
                bool& facing_left) {

    //the position of the other entity
    int ox,oy;

    //look through entities in the game
    for (size_t i=0; i<pos.size(); i++) {
      ox = std::get<EPOS_X>(pos.at(i));
      oy = std::get<EPOS_Y>(pos.at(i));

      //go towards the player if sufficiently far away
      if (std::get<EPOS_PLAYER>(pos.at(i))) {
        //always face towards the player
        facing_left = (ox < x);

        //don't stop if midway through climbing
        if (state != CLIMB) {
          //check the close distance to the player
          if (distance(x,y,ox,oy) > SURVEYOR_FOLLOW_DIST) {
            //walk towards the player
            if ((state != CLIMB) &&
                (std::get<EPOS_STATE>(pos.at(i)) != CLIMB)) {
              state = MOVE;
            }
          } else {
            //idle
            state = IDLE;
          }
        }
      }
    }
  }

}}}
