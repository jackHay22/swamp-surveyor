/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "proc_generation.h"
#include <variant>
#include "../logger.h"
#include <iostream>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <cmath>

namespace impl {
namespace environment {
namespace proc_generation {

  #define FORWARD "F"
  #define EXPAND "X"
  #define ANGLE_PLUS "+"
  #define ANGLE_MINUS "-"
  #define PI 3.14159265
  #define MAX_FRAMES 6 //max tree frames

  //the current state of the l system
  struct l_system_state {
    std::vector<std::variant<std::string,l_system_state>> s;

    std::string debug() {
      std::string contents = "";

      for (size_t i=0; i<s.size(); i++) {
        try {
          contents += std::get<std::string>(s.at(i));
        } catch (const std::bad_variant_access&) {
          try {
            contents += "[" + std::get<l_system_state>(s.at(i)).debug() + "]";
          } catch (const std::bad_variant_access&) {}
        }
      }
      return contents;
    }
  };

  /**
   * Expand the l system state for some number of iterations
   * @param state the state of the l system
   * @param x_expansion what x expands to (minus the first F)
   * @param iters the iterations
   */
  void expand(l_system_state& state, const l_system_state& x_expansion, int iters) {
    for (int i=0; i<iters; i++) {

      //state to copy into
      l_system_state expanded_state;

      //expand the l system state
      for (size_t i=0; i<state.s.size(); i++) {
        try {
          //try to get as string
          std::string curr = std::get<std::string>(state.s.at(i));

          //Note: We ignore +,- for now
          if (curr == EXPAND) {
            std::copy(x_expansion.s.begin(),
                      x_expansion.s.end(),
                      back_inserter(expanded_state.s));

          } else if (curr == FORWARD) {
            expanded_state.s.push_back(FORWARD);
            expanded_state.s.push_back(FORWARD);

          } else {
            // +/-
            expanded_state.s.push_back(curr);
          }

        } catch (const std::bad_variant_access&) {
          try {
            l_system_state expand_grp = std::get<l_system_state>(state.s.at(i));

            if (!expand_grp.s.empty()) {
              //expand recursively
              expand(expand_grp,x_expansion,1);

              //add in the expanded group
              expanded_state.s.push_back(expand_grp);
            }

          } catch (const std::bad_variant_access&) {
            logger::log_err("bad variant access on both string and l_system");
          }
        }
      }

      //update state
      state = expanded_state;
    }
  }

  /**
   * Render the l system component
   * @param state       the l system state to render from
   * @param constructor the texture constructor to add to
   * @param x           position x
   * @param y           position y
   * @param dist        the distance in the forward direction
   * @param angle       the current angle
   */
  void render(const l_system_state& state,
              texture_constructor_t& constructor,
              int x, int y,
              int dist, int angle) {

    int curr_angle = angle;
    int curr_x = x;
    int curr_y = y;

    int forward_count = 0;

    //execute the lsystem commands
    for (size_t i=0; i<state.s.size(); i++) {
      try {
        const std::string& command = std::get<std::string>(state.s.at(i));

        //check the command type
        if (command == FORWARD) {
          forward_count++;

        } else {

          //check if we should move forward some distance
          if (forward_count > 0) {
            int move_dist = (dist * forward_count);

            //move along the angle, draw a line between the points
            int new_x = curr_x + (move_dist * sin(curr_angle * PI / 180.0));
            int new_y = curr_y - (move_dist * cos(curr_angle * PI / 180.0));

            //draw a line
            constructor.set_line(curr_x,curr_y,new_x,new_y,1);

            //update the current position
            curr_x = new_x;
            curr_y = new_y;

            //reset
            forward_count = 0;
          }

          if (command == ANGLE_MINUS) {
            curr_angle -= ((rand() % 5) + 20);

          } else if (command == ANGLE_PLUS) {
            curr_angle += ((rand() % 5) + 20);
          }
        }

      } catch (const std::bad_variant_access&) {
        try {

          //render this group
          render(
            std::get<l_system_state>(state.s.at(i)),
            constructor,
            curr_x, curr_y,
            dist,
            curr_angle
          );

        } catch (const std::bad_variant_access&) { }
      }
    }
  }

  /**
   * Execute a fractal plant l system on this texture constructor
   * @param constructor the texture constructor
   * @param iters       the number of iterations to evaluate for
   * @param r           color r
   * @param g           color g
   * @param b           color b
   * @param x           position x
   * @param y           position y
   */
  void fractal_tree_lsystem(texture_constructor_t& constructor,
                            int iters,
                            int r, int g, int b,
                            int x, int y) {
    l_system_state state;
    state.s.push_back(EXPAND);

    // X -> F+[-F-XF-X][+FF][--XF[+X]][++F-X]
    // F -> FF
    l_system_state x_expansion;
    x_expansion.s.push_back(FORWARD);
    x_expansion.s.push_back(ANGLE_PLUS);
    l_system_state grp1;
    grp1.s.push_back(ANGLE_MINUS);
    grp1.s.push_back(FORWARD);
    grp1.s.push_back(ANGLE_MINUS);
    grp1.s.push_back(EXPAND);
    grp1.s.push_back(FORWARD);
    grp1.s.push_back(ANGLE_MINUS);
    grp1.s.push_back(EXPAND);
    x_expansion.s.push_back(grp1);
    l_system_state grp2;
    grp2.s.push_back(ANGLE_PLUS);
    grp2.s.push_back(FORWARD);
    grp2.s.push_back(FORWARD);
    x_expansion.s.push_back(grp2);
    l_system_state grp3;
    grp3.s.push_back(ANGLE_MINUS);
    grp3.s.push_back(ANGLE_MINUS);
    grp3.s.push_back(EXPAND);
    grp3.s.push_back(FORWARD);
    l_system_state grp4;
    grp4.s.push_back(ANGLE_PLUS);
    grp4.s.push_back(EXPAND);
    grp3.s.push_back(grp4);
    x_expansion.s.push_back(grp3);
    l_system_state grp5;
    grp5.s.push_back(ANGLE_PLUS);
    grp5.s.push_back(ANGLE_PLUS);
    grp5.s.push_back(FORWARD);
    grp5.s.push_back(ANGLE_MINUS);
    grp5.s.push_back(EXPAND);
    x_expansion.s.push_back(grp5);

    //expand the l system
    expand(state,x_expansion,iters);

    int dist = 2;
    int angle = 0;

    //set the default color
    constructor.set_default_color(r,g,b);

    //render the state onto the constructor
    render(state,constructor,x,y,dist,angle); //state
  }

  /**
   * Add a leaf at a given position
   * @param constructor the texture constructor
   * @param x           position x
   * @param y           position y
   * @param r           color r
   * @param g           color g
   * @param b           color b
   */
  void add_leaf(texture_constructor_t& constructor,
                int x, int y,
                int r, int g, int b) {
    //leaf center
    constructor.set(x,y,r,g,b);
    constructor.set(x-1,y,r,g,b);
    constructor.set(x+1,y,r,g,b);
    constructor.set(x,y-1,r,g,b);
    constructor.set(x,y+1,r,g,b);

    //a few randoms
    for (int i=0; i<4; i++) {
      int px = x + (rand() % 4 - 2);
      int py = y + (rand() % 4 - 2);

      //add to some of the frames
      for (int f=0; f<MAX_FRAMES; f++) {
        if (rand() % MAX_FRAMES != 0) {
          constructor.set(px,py,r,g,b,f);
        }
      }
    }
  }

  /**
   * The distance between two points (rough)
   * @return    distance
   */
  int distance(int x1, int y1, int x2, int y2) {
    return std::sqrt(
            pow(std::abs(x1 - x2),2) +
            pow(std::abs(y1 - y2),2));
  }

  /**
   * Generate trunk foliage given a pre constructed trunk texture
   * @param constructor the constructor that already has a trunk
   * @param count       the number of leaves (density)
   * @param r           leaf color r
   * @param g           leaf color g
   * @param b           leaf color b
   * @param two_color   whether to use two colors for leaves
   * @param r2          leaf color r (second)
   * @param g2          leaf color g (second)
   * @param b2          leaf color b (second)
   */
  void trunk_foliage(texture_constructor_t& constructor,
                     int count,
                     int r, int g, int b,
                     bool two_color,
                     int r2, int g2, int b2) {

    //clamp the constructor to the trunk
    constructor.clamp(10);

    //the starting trunk position
    const int cx = constructor.get_width() / 2;
    const int cy = constructor.get_height() / 3;

    int range_x = cx;
    int range_y = constructor.get_height() / 2;

    int rem = count;
    while (rem > 0) {
      int x = cx + (rand() % (2 * range_x) - range_x);
      int y = cy + (rand() % (2 * range_y) - range_y);

      if (constructor.is_set(x,y) ||
          ((rand() % 50 == 0) && (distance(cx,cy,x,y) < (cx / 2)))) {
        int set_x = x + (rand() % 10 - 5);
        int set_y = y + (rand() % 10 - 5);
        //add a leaf at this position
        add_leaf(constructor,set_x,set_y,r,g,b);
        rem--;

        if (two_color) {
          //TODO
        }
      }
    }
  }

  /**
   * Branch out at an angle
   * @param constructor texture constructor
   * @param x           branch position x
   * @param y           branch position y
   * @param angle       angle to branch at
   * @param volume      volume of this branch
   * @param frame       the animation frame
   */
  void branch_at_angle(texture_constructor_t& constructor,
                       int x, int y,
                       int angle,
                       float volume,
                      int frame);

  /**
   * Create an animated branch
   * @param constructor texture constructor
   * @param x           position x
   * @param y           position y
   * @param angle       angle
   * @parm volume       the volume of the branch
   */
  void animated_branch_at_angle(texture_constructor_t& constructor,
                                int x, int y,
                                int angle,
                                float volume) {
    //add the animation for the first set
    for (int i=0; i<(MAX_FRAMES / 2); i++) {
      branch_at_angle(
        constructor,
        x,y,angle,
        volume,
        i
      );
    }

    //add the animation for the second set
    for (int i=(MAX_FRAMES / 2); i<MAX_FRAMES; i++) {
      branch_at_angle(
        constructor,
        x + 1,
        y,angle,
        volume,
        i
      );
    }
  }

  /**
   * Branch out at an angle
   * @param constructor texture constructor
   * @param x           branch position x
   * @param y           branch position y
   * @param angle       angle to branch at
   * @param volume      volume of this branch
   * @param frame       the animation frame
   */
  void branch_at_angle(texture_constructor_t& constructor,
                       int x, int y,
                       int angle,
                       float volume,
                       int frame) {
    //branch distance is a function of "volume"
    int dist = ceil(volume) * 4;

    //the next branch position
    int brx = x + (dist * sin(angle * PI / 180.0));
    int bry = y - (dist * cos(angle * PI / 180.0));

    //draw a line
    constructor.set_line(x,y,brx,bry,(int) ceil(volume),frame);

    if (volume > 0.1) {
      float volume_l = static_cast<float>(rand()) / static_cast<float>(RAND_MAX/volume);
      float volume_r = volume - volume_l;

      if (volume_l > 0.0) {

        // if (volume_l <= 0.2) {
        //   animated_branch_at_angle(
        //     constructor,
        //     brx,bry,
        //     angle - (rand() % 25) + 1,
        //     volume_l
        //   );
        //
        // } else {
          //left branch
          branch_at_angle(
            constructor,
            brx,bry,
            angle - (rand() % 25) + 1,
            volume_l,
            frame
          );
        //}
      }

      if (volume_r > 0.0) {
        // if (volume_r <= 0.2) {
        //   animated_branch_at_angle(
        //     constructor,
        //     brx,bry,
        //     angle - (rand() % 25) + 1,
        //     volume_r
        //   );
        //
        // } else {
          //right branch
          branch_at_angle(
            constructor,
            brx,bry,
            angle + (rand() % 25) + 1,
            volume_r,
            frame
          );
        //}
      }
    }
  }

  /**
   * Build a tree trunk by branching and conserving "volume"
   * @param constructor texture constructor
   * @param x,y position
   * @param r,g,b the color of the trunk
   */
  void branching_tree_growth(texture_constructor_t& constructor,
                             int x, int y,
                             int r, int g, int b) {
    constructor.set_default_color(r,g,b);
    //trunk starts with volume 4
    branch_at_angle(constructor,x,y,0,4.0,-1);
  }

}}}
