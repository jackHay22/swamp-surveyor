/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "anim_set.h"
#include <json/nlohmann_json.h>
#include <fstream>
#include "../exceptions.h"
#include "../utils.h"
#include "../logger.h"

namespace impl {
namespace entity {

  typedef nlohmann::json json;

  //animation configuration structure
  struct anim_cfg_t {
    //the path to the resource
    std::string rsrc_path = "<undefined>";
    //the width of each frame
    int frame_width = 0;
    //the height of each frame
    int frame_height = 0;
    //the number of frames in the animation
    int anim_frames = 8;
    //the duration of each frame in ticks
    int duration = 1;
  };

  /**
   * Conversion from cfg to json
   * @param j the json
   * @param c the cfg to convert
   */
  void to_json(json& j, const anim_cfg_t& c) {
    j = json{{"rsrc_path", c.rsrc_path},
             {"frame_width", c.frame_width},
             {"frame_height", c.frame_height},
             {"anim_frames", c.anim_frames},
             {"duration", c.duration}};
  }

  /**
   * Conversion to cfg from json
   * @param j the json to load
   * @param c the cfg to load into
   */
  void from_json(const json& j, anim_cfg_t& c) {
    j.at("rsrc_path").get_to(c.rsrc_path);
    j.at("frame_width").get_to(c.frame_width);
    j.at("frame_height").get_to(c.frame_height);
    j.at("anim_frames").get_to(c.anim_frames);
    j.at("duration").get_to(c.duration);
  }


  /**
   * Construct the animation set
   * @param path            the resource path
   * @param renderer        the renderer for loading images
   */
  anim_set_t::anim_set_t(const std::string& path,
                         SDL_Renderer& renderer)
    : current_frame(0) {

    //read from the configuration file
    std::ifstream in_stream(path);
    nlohmann::json config;

    try {
      in_stream >> config;
      anim_cfg_t cfg = config.get<anim_cfg_t>();

      //initialize from values in cfg
      this->frame_width = cfg.frame_width;
      this->frame_height = cfg.frame_height;
      this->anim_frames = cfg.anim_frames;
      this->duration = cfg.duration;

      //load the texture
      int width = this->frame_width * this->anim_frames;

      //load the texture
      this->texture = utils::load_texture(cfg.rsrc_path,
                                          renderer, width,
                                          this->frame_height);

    } catch (...) {
      throw exceptions::rsrc_exception_t("failed to load animation from cfg: " + path);
    }
  }

  /**
  * Free texture
  */
  anim_set_t::~anim_set_t() {
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
    }
  }

  /**
  * Update the animation
  */
  void anim_set_t::update() {
    ticks++;

    if (ticks >= duration) {
      ticks = 0;
      current_frame++;
      current_frame %= anim_frames;
    }
  }

  /**
  * Render the animation frame
  * @param renderer the renderer to use
  * @param x        the x position
  * @param y        the y position
  */
  void anim_set_t::render(SDL_Renderer& renderer, int x, int y) const {
    //create a clip for the current frame
    SDL_Rect sample_bounds;
    //determine the coords of the tile within the set
    sample_bounds.x = this->current_frame * this->frame_width;
    sample_bounds.y = 0;
    sample_bounds.w = this->frame_width;
    sample_bounds.h = this->frame_height;

    //the x y position to render at
    SDL_Rect image_bounds = {x, y,
                             x + this->frame_width,
                             y + this->frame_height};

    //render the current animation frame
    SDL_RenderCopyEx(&renderer,
                     this->texture,
                     &sample_bounds,
                     &image_bounds,
                     0,NULL,SDL_FLIP_NONE);
  }
}}
