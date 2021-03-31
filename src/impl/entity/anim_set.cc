/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "anim_set.h"
#include <json/nlohmann_json.h>
#include <fstream>
#include <iostream>
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
   * Constructor
   * @param path      the path to the texture
   * @param renderer  the renderer for loading the texture
   * @param base_path the path to the resources folder
   */
  anim_set_t::anim_set_t(const std::string& path,
                         SDL_Renderer& renderer,
                         const std::string& base_path)
    : current_frame(0) {

    //read from the configuration file
    std::ifstream in_stream(base_path + path);
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
      this->texture = utils::load_texture(base_path + cfg.rsrc_path,
                                          renderer, width,
                                          this->frame_height);

    } catch (...) {
      throw exceptions::rsrc_exception_t("failed to load animation from cfg: " + base_path + path);
    }
  }

  /**
   * Construct from a loaded texture
   * @param texture the loaded texture (anim set assumes ownership)
   * @param w       the width of the texture
   * @param h       the height of the texture
   * @param frames  the number of frames
   */
  anim_set_t::anim_set_t(SDL_Texture *texture, int w, int h, int frames, int duration)
    : anim_frames(frames),
      frame_width(w / frames),
      frame_height(h),
      duration(duration),
      texture(texture) {}

  /**
  * Free texture
  */
  anim_set_t::~anim_set_t() {
    if (texture != NULL) {
      SDL_DestroyTexture(texture);
      texture = NULL;
    }
  }

  /**
   * Set the animation slower
   * @param is_slow the value
   */
  void anim_set_t::set_slow(bool is_slow) {
    //adjust slowness
    if (is_slow && !this->is_slow) {
      this->duration++;
    } else if (!is_slow && this->is_slow) {
      this->duration--;
    }

    this->is_slow = is_slow;
  }

  /**
  * Update the animation
  */
  void anim_set_t::update() {
    ticks++;

    if (ticks >= duration) {
      ticks = 0;

      //check to see if frame should be updated
      if (!(once && (current_frame == (anim_frames - 1)))) {
        current_frame++;
        current_frame %= anim_frames;
      }
    }
  }

  /**
  * Render the animation frame
  * @param renderer the renderer to use
  * @param x        the x position
  * @param y        the y position
  * @param facing_left whether the animation is facing left
  */
  void anim_set_t::render(SDL_Renderer& renderer,
                          int x, int y,
                          bool facing_left) const {
    //create a clip for the current frame
    SDL_Rect sample_bounds;
    //determine the coords of the tile within the set
    sample_bounds.x = this->current_frame * this->frame_width;
    sample_bounds.y = 0;
    sample_bounds.w = this->frame_width;
    sample_bounds.h = this->frame_height;

    //the x y position to render at
    SDL_Rect image_bounds = {x - (frame_width / 2),
                             y - (frame_height / 2),
                             this->frame_width,
                             this->frame_height};

    if (texture != NULL) {
      if (facing_left) {
        //flip and render the current frame
        SDL_RenderCopyEx(&renderer,
                         this->texture,
                         &sample_bounds,
                         &image_bounds,
                         0, NULL,
                         SDL_FLIP_HORIZONTAL);
      } else {
        //render the current animation frame
        SDL_RenderCopy(&renderer,
                       this->texture,
                       &sample_bounds,
                       &image_bounds);
      }
    }
  }
}}
