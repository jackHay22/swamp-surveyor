/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "insects.h"
#include <json/nlohmann_json.h>
#include "../exceptions.h"
#include <fstream>
#include <stdlib.h>

namespace impl {
namespace entity {

  typedef nlohmann::json json;

  //json configuration
  struct insect_cfg_t {
    //the number of insects
    int count = 0;
    //alternations between insect and non insect segments
    std::vector<int> divisions = {};
    //the max height the insects can range
    int bounds_top = 0;
    int bounds_bot = 0;
    //the color of insects
    int r = 0;
    int b = 0;
    int g = 0;
  };

  /**
   * Conversion to cfg from json
   * @param j the json to load
   * @param c the cfg to load into
   */
  void from_json(const json& j, insect_cfg_t& c) {
    j.at("count").get_to(c.count);
    j.at("divisions").get_to(c.divisions);
    j.at("bounds_top").get_to(c.bounds_top);
    j.at("bounds_bot").get_to(c.bounds_bot);
    j.at("r").get_to(c.r);
    j.at("b").get_to(c.b);
    j.at("g").get_to(c.g);
  }

  /**
   * Initialize map wide insect swarm
   * @param cfg_path the path to the insect configuration file
   */
  insects_t::insects_t(const std::string& cfg_path) {

    try {
      //init random seed
      srand(time(NULL));

      std::ifstream in_stream(cfg_path);
      nlohmann::json config;

      in_stream >> config;
      insect_cfg_t cfg = config.get<insect_cfg_t>();

      //load from configuration
      this->bounds_top = cfg.bounds_top;
      this->bounds_bot = cfg.bounds_bot;
      this->r = cfg.r;
      this->b = cfg.b;
      this->g = cfg.g;

      //check for even number of divisions
      if ((cfg.divisions.size() % 2) != 0) {
        throw exceptions::rsrc_exception_t(cfg_path,"odd # of division endpts");
      }

      //create divisions
      for (size_t i=0; i<cfg.divisions.size(); i+=2) {
        this->bounds_w.push_back(std::make_pair(cfg.divisions.at(i),
                                                cfg.divisions.at(i+1)));
      }

      if (!this->bounds_w.empty()) {
        //the number of insects (roughly) per section
        int insects_per_section = cfg.count / this->bounds_w.size();

        //for each group
        for (size_t i=0; i<this->bounds_w.size(); i++) {

          //add segment of insects
          for (int j=0; j<insects_per_section; j++) {
            //add a random position
            positions.push_back(
              std::make_pair((rand() % this->bounds_w.at(i).second) +
                                    this->bounds_w.at(i).first,
                             (rand() % bounds_bot) + bounds_top));
          }
        }
      }

    } catch (...) {
      throw exceptions::rsrc_exception_t(cfg_path);
    }
  }

  /**
   * Check if a given point lies within the rectangle provided
   * @param pt the point to check
   * @param camera the rectangle
   * @return whether the point is in the rectangle
   */
  bool insects_t::in_camera(std::pair<int,int> pt, const SDL_Rect& camera) const {
    return (pt.first > camera.x) &&
           (pt.first < (camera.x + camera.w)) &&
           (pt.second > camera.y) &&
           (pt.second < (camera.y + camera.h));
  }

  /**
   * Update the cloud of insects
   */
  void insects_t::update() {
    //update each insect
    for (size_t i=0; i<positions.size(); i++) {
      //only update sometimes
      if ((rand() % 2) == 0) {
        int dx = (rand() % 3) - 1;
        int dy = (rand() % 3) - 1;

        //store the previous x value
        int prev_x = positions.at(i).first;

        //compute a new position
        positions.at(i).first += dx;
        positions.at(i).second += dy;

        //force insects to stay in region
        if (positions.at(i).second < bounds_top) {
          positions.at(i).second += 2;
        } else if (positions.at(i).second > bounds_bot) {
          positions.at(i).second -= 2;
        }

        //check to make sure the insect has not left a bound box
        for (size_t j=0; j<bounds_w.size(); j++) {
          //check if this is the correct bounding box
          if ((prev_x >= bounds_w.at(j).first) &&
              (prev_x < bounds_w.at(j).second)) {

            //check if insect now above
            if (positions.at(i).first >= bounds_w.at(j).second) {
              positions.at(i).first -= 2;

            } else if (positions.at(i).first < bounds_w.at(j).first) {
              positions.at(i).first += 2;
            }

            continue;
          }
        }
      }
    }
  }

  /**
   * Render the insects
   * @param renderer the renderer to use
   * @param camera   the camera
   * @param debug    whether debug mode enabled
   */
  void insects_t::render(SDL_Renderer& renderer,
                         const SDL_Rect& camera,
                         bool /*debug*/) const {
    //set the insect color
    SDL_SetRenderDrawColor(&renderer,this->r,this->g,this->b,225);

    //render each insect
    for (size_t i=0; i<positions.size(); i++) {
      //check if the point is in view
      if (in_camera(positions.at(i), camera)) {
        //render a pixel
        SDL_RenderDrawPoint(&renderer,
                            positions.at(i).first - camera.x,
                            positions.at(i).second - camera.y);
      }
    }
  }

}}
