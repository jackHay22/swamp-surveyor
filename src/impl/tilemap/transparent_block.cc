/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "transparent_block.h"
#include "../utils.h"
#include "../exceptions.h"
#include <json/nlohmann_json.h>
#include <fstream>
#include <algorithm>

namespace impl {
namespace tilemap {

  #define DEFAULT_R 8
  #define DEFAULT_G 17
  #define DEFAULT_B 0

  typedef nlohmann::json json;

  //the configuration of each block
  struct tpb_cfg {
    int x;
    int y;
    int w;
    int h;
    bool texture;
    std::string texture_path;
  };

  /**
   * Conversion to cfg from json
   * @param j the json to load
   * @param c the cfg to load into
   */
  void from_json(const json& j, tpb_cfg& c) {
    j.at("x").get_to(c.x);
    j.at("y").get_to(c.y);
    j.at("w").get_to(c.w);
    j.at("h").get_to(c.h);
    j.at("texture").get_to(c.texture);
    j.at("texture_path").get_to(c.texture_path);
  }

  /**
   * Load transparent blocks
   * @param blocks   the blocks vec set by the call
   * @param path     the path to the configuration file
   * @param renderer the renderer for loading textures
   * @param debug    whether debug mode enabled
   */
  void mk_transparent_blocks(std::vector<std::shared_ptr<transparent_block_t>>& blocks,
                             const std::string& path,
                             SDL_Renderer& renderer,
                             bool debug) {
    try {
      nlohmann::json config;
      std::ifstream in_stream(path);
      in_stream >> config;

      //load each env element
      for (json& env : config) {
        //load the configuration
        tpb_cfg cfg = env.get<tpb_cfg>();

        //check for mode
        if (cfg.texture) {
          blocks.push_back(std::make_shared<transparent_block_t>(cfg.x,
                                                                 cfg.y,
                                                                 cfg.w,
                                                                 cfg.h,
                                                                 cfg.texture_path,
                                                                 renderer,
                                                                 debug));
        } else {
          blocks.push_back(std::make_shared<transparent_block_t>(cfg.x,
                                                                 cfg.y,
                                                                 cfg.w,
                                                                 cfg.h,
                                                                 DEFAULT_R,
                                                                 DEFAULT_G,
                                                                 DEFAULT_B,
                                                                 debug));
        }
      }

    } catch (...) {
      throw exceptions::rsrc_exception_t(path);
    }
  }



  /**
   * Constructor for texture version
   * @param x            position x
   * @param y            position y
   * @param w            width
   * @param h            height
   * @param texture_path path to texture
   * @param renderer     renderer for loading texture
   * @param debug        debug mode enabled
   */
  transparent_block_t::transparent_block_t(int x, int y, int w, int h,
                                           const std::string& texture_path,
                                           SDL_Renderer& renderer,
                                           bool debug)
    : bounds({x,y,w,h}),
      r(0), g(0), b(0),
      debug(debug),
      transparent(false) {
    //load the texture
    texture = utils::load_texture(texture_path,
                                  renderer,
                                  texture_w,
                                  texture_h);
  }

  /**
   * Constructor for color version
   * @param x     position x
   * @param y     position y
   * @param w     width
   * @param h     height
   * @param r     color r
   * @param g     color g
   * @param b     color b
   * @param debug debug mode enabled
   */
  transparent_block_t::transparent_block_t(int x, int y, int w, int h,
                                           int r, int g, int b,
                                           bool debug)
    : bounds({x,y,w,h}),
      texture_w(0),
      texture_h(0),
      r(r), g(g), b(b),
      debug(debug),
      transparent(false) {}

  /**
   * Free texture if applicable
   */
  transparent_block_t::~transparent_block_t() {
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }

  /**
   * Check if a bounding box collides with this block
   * @param  other the other bounding box
   * @param max_dim whether to compare texture and bounding box and
   * consider max
   * @return       whether the two boxes collide
   */
  bool transparent_block_t::is_collided(const SDL_Rect& other,
                                        bool max_dim) const {
    int actual_w = bounds.w;
    int actual_h = bounds.h;

    //if max dim set, find max width and height
    if (max_dim) {
      actual_w = std::max(actual_w,texture_w);
      actual_h = std::max(actual_h,texture_h);
    }

    int actual_x = bounds.x - (actual_w / 2);
    int actual_y = bounds.y - (actual_h / 2);

    return ((actual_x < (other.x + other.w)) &&
            ((actual_x + actual_w) > other.x) &&
            (actual_y < (other.y + other.h)) &&
            ((actual_y + actual_h) > other.y));
  }

  /**
   * Update the block. If the player intersects,
   * set transparent
   * @param player the bounding box for the player
   */
  void transparent_block_t::update(const SDL_Rect& player) {
    this->transparent = this->is_collided(player);
  }

  /**
   * Render this block
   * @param renderer the sdl renderer
   * @param camera the camera
   */
  void transparent_block_t::render(SDL_Renderer& renderer,
                                  const SDL_Rect& camera) const {
    if (this->is_collided(camera,true) && !transparent) {

      if (texture != NULL) {
        //render the texture

        //create a clip for the current frame
        SDL_Rect sample_bounds = {0,0,texture_w,texture_h};

        //the x y position to render at
        SDL_Rect image_bounds = {(bounds.x - (texture_w / 2)) - camera.x,
                                 (bounds.y - (texture_h / 2)) - camera.y,
                                 texture_w,texture_h};

        //render the texture
        SDL_RenderCopy(&renderer,
                       texture,
                       &sample_bounds,
                       &image_bounds);
      } else {
        SDL_SetRenderDrawColor(&renderer,r,g,b,255);

        SDL_Rect fill_rect = {(bounds.x - (bounds.w / 2)) - camera.x,
                              (bounds.y - (bounds.h / 2)) - camera.y,
                              bounds.w,bounds.h};
        //render rect
        SDL_RenderFillRect(&renderer,&fill_rect);
      }

      if (debug) {
        //render debug rect
        SDL_Rect debug_bounds = {(bounds.x - (bounds.w / 2)) - camera.x,
                                 (bounds.y - (bounds.h / 2)) - camera.y,
                                 bounds.w, bounds.h};
        //set the draw color
        SDL_SetRenderDrawColor(&renderer,255,255,0,255);
        //render the bounds
        SDL_RenderDrawRect(&renderer,&debug_bounds);
      }
    }
  }
}}
