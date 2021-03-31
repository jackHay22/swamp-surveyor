/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "texture_constructor.h"
#include "../logger.h"
#include <iostream>
#include <cmath>
#include <algorithm>

namespace impl {
namespace environment {

  #define ALPHA_CHAN 255

  /**
   * Constructor
   */
  texture_constructor_t::texture_constructor_t()
    : w(0), h(0),
      min_y(INT_MAX),
      min_x(INT_MAX),
      r(0), g(0), b(0),
      frame_sets() {}

  /**
   * Execute some function on each pixel
   * @param fn the function
   */
  void texture_constructor_t::for_each(std::function<bool(px_t&)> fn) {
    for (size_t i=0; i<frame_sets.size(); i++) {
      //execute fn on all pixels in set
      for (std::map<pos_t,px_t>::iterator it = frame_sets.at(i)->begin(); it != frame_sets.at(i)->end(); ++it) {
        if (!fn(it->second)) {
          return;
        }
      }
    }
  }

  /**
   * Execute some function on each pixel
   * If the function returns false, we exit the loop
   * @param fn the function
   */
  void texture_constructor_t::for_each(std::function<bool(const px_t&)> fn) const {
    for (size_t i=0; i<frame_sets.size(); i++) {
      //execute fn on all pixels in set
      for (std::map<pos_t,px_t>::const_iterator it = frame_sets.at(i)->begin(); it != frame_sets.at(i)->end(); ++it) {
        if (!fn(it->second)) {
          return;
        }
      }
    }
  }

  /**
   * Get the frame for a given index (create if not yet set)
   * @param  frame frame index
   * @return       the frame
   */
  frame_t& texture_constructor_t::get_frame(size_t frame) {
    while (frame_sets.size() <= frame) {
      //create a new frame
      frame_sets.push_back(std::make_unique<frame_t>());
    }

    //get a deref
    return *frame_sets.at(frame);
  }

  /**
   * Clamp the bounds of the texture based on pixels added so far
   */
  void texture_constructor_t::clamp(int buffer) {

    //the adjustment to make
    int x_adjust = min_x - buffer;
    int y_adjust = min_y - buffer;

    //adjust the position of each pixel
    for_each([x_adjust,y_adjust](px_t& px) {
      std::get<PINFO_POS>(px).first -= x_adjust;
      std::get<PINFO_POS>(px).second -= y_adjust;

      //continue
      return true;
    });

    w -= x_adjust;
    h -= y_adjust;
    min_x -= x_adjust;
    min_y -= y_adjust;
  }

  /**
   * Check if a given position has a texture already
   * @param  x pos x
   * @param  y pos y
   * @return   whether a pixel is set at this position already
   */
  bool texture_constructor_t::is_set(int x, int y) const {

    bool set = false;

    //iterate over pixels
    for_each([x,y,&set](const px_t& px) {
      if ((std::get<PINFO_POS>(px).first == x) &&
          (std::get<PINFO_POS>(px).second == y)) {
        set = true;
        return false;
      }
      //keep looking
      return true;
    });

    return set;
  }

  /**
   * Set the default colors to use
   * @param r r channel
   * @param g g channel
   * @param b b channel
   */
  void texture_constructor_t::set_default_color(Uint8 r,Uint8 g,Uint8 b) {
    this->r = r;
    this->g = g;
    this->b = b;
  }

  /**
   * Set default color at some position
   * @param x position x
   * @param y position y
   * @param frame the frame to set this pixel in
   */
  void texture_constructor_t::set(int x, int y, int frame) {
    this->set(x,y,r,g,b,frame);
  }

  /**
   * Set a rectangle, takes position, dimension and frame
   */
  void texture_constructor_t::set_rect(int x, int y, int w, int h, int frame) {
    for (int i=x; i<(x + w); i++) {
      for (int j=y; j<(y + h); j++) {
        //set the pixel at this position
        this->set(i,j,frame);
      }
    }
  }

  /**
   * Erase the pixel at a given position (if exists)
   */
  void texture_constructor_t::erase(int x, int y, int frame) {
    //get the frame
    frame_t& f = (frame < 0) ? get_frame(0) : get_frame(frame);
    std::map<pos_t,px_t>::iterator it = f.find(std::make_pair(x,y));

    if (it != f.end()) {
      f.erase(it);
    }
  }

  /**
   * Render a line between two points
   * https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
   * http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C.2B.2B
   */
  void texture_constructor_t::set_line(int x1, int y1, int x2, int y2, int thickness, int frame) {
    bool is_steep = std::abs(y2 - y1) > std::abs(x2 - x1);

    if (is_steep) {
      std::swap(x1,y1);
      std::swap(x2,y2);
    }

    if(x1 > x2) {
      std::swap(x1, x2);
      std::swap(y1, y2);
    }

    float dx = x2 - x1;
    float dy = std::abs(y2 - y1);

    float err = dx / 2.0f;
    int y_step = -1;

    if (y1 < y2) {
      y_step = 1;
    }

    int y = (int)y1;
    int max_x = (int)x2;

    for (int x=(int)x1; x<max_x; x++) {
      if (is_steep) {
        set(y,x,frame);
        for (int i=1; i<thickness; i++) {
          set(y+i,x,frame);
        }
      } else {
        set(x,y,frame);
        for (int i=1; i<thickness; i++) {
          set(x+i,y,frame);
        }
      }

      err -= dy;
      if(err < 0) {
        y += y_step;
        err += dx;
      }
    }
  }

  /**
   * Set the color of a pixel at some position
   * @param x the position in the texture x
   * @param y the position in the texture y
   * @param r color of the pixel r
   * @param g color of the pixel g
   * @param b color of the pixel b
   * @param frame the frame to set this pixel in
   */
  void texture_constructor_t::set(int x, int y, Uint8 r, Uint8 g, Uint8 b, int frame) {

    if ((x < 0) || (y < 0)) {
      return;
    }

    //adjust dimensions
    if (x >= w) {
      w = x + 1;
    }
    if (y >= h) {
      h = y + 1;
    }
    if (y < min_y) {
      min_y = y;
    }
    if (x < min_x) {
      min_x = x;
    }

    std::pair<int,int> pos = std::make_pair(x,y);

    if (frame < 0) {
      //insert into zero (the generate call handles replication)
      get_frame(0).insert_or_assign(
        pos,
        std::make_tuple(pos,frame,std::make_tuple(r,g,b))
      );

    } else {
      //insert the pixel info
      get_frame(frame).insert_or_assign(
        pos,
        std::make_tuple(pos,frame,std::make_tuple(r,g,b))
      );
    }
  }

  /**
   * Generate the texture
   * @param  renderer the renderer for loading the texture
   * @param  w        the width of the texture set by the call
   * @param  h        the height of the texture set by the call
   * @return the      texture
   */
  SDL_Texture* texture_constructor_t::generate(SDL_Renderer& renderer, int& w, int& h) const {
    if (frame_sets.empty() || frame_sets.at(0)->empty()) {
      logger::log_err("cannot create texture from empty pixel set");
      exit(1);
    }

    SDL_Surface *surface;
    //r, g, b, alpha masks
    Uint32 rmask, gmask, bmask, amask;

    //https://wiki.libsdl.org/SDL_CreateRGBSurface

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    //create the surface (adjust height based on the min y, min x values, number of animation frames)
    surface = SDL_CreateRGBSurface(0,
                                   (this->w - min_x) * get_frames(),
                                   (this->h - min_y),
                                   32,
                                   rmask,
                                   gmask,
                                   bmask,
                                   amask);
    if (surface == NULL) {
      logger::log_err("failed to create RGB surface: " +
                        std::string(SDL_GetError()));
      exit(1);
    }

    //edit the pixel data
    SDL_LockSurface(surface);

    //iterate over pixels
    for_each([&surface,this](const px_t& px) {

      //generate the pixel rgba data based on the surface format (depends on endianess)
      Uint32 pfmt = SDL_MapRGBA(surface->format,
                                std::get<0>(std::get<PINFO_RGB>(px)),
                                std::get<1>(std::get<PINFO_RGB>(px)),
                                std::get<2>(std::get<PINFO_RGB>(px)), ALPHA_CHAN);

      //check if we add to all frames
      if (std::get<PINFO_FRAME>(px) == -1) {

        //add to each frame
        for (size_t f=0; f<this->frame_sets.size(); f++) {
          //get the pixel position in the surface buffer and set
          Uint8 *surface_pixel = (Uint8*)surface->pixels;
          surface_pixel += ((std::get<PINFO_POS>(px).second - this->min_y) * surface->pitch) +
                           (((std::get<PINFO_POS>(px).first - this->min_x) + ((this->w - this->min_x) * f))
                            * sizeof(Uint32));
          *((Uint32*)surface_pixel) = pfmt;
        }

      } else { //set in a specific frame
        //get the pixel position in the surface buffer and set
        Uint8 *surface_pixel = (Uint8*)surface->pixels;
        surface_pixel += ((std::get<PINFO_POS>(px).second - this->min_y) * surface->pitch) +
                         (((std::get<PINFO_POS>(px).first - this->min_x) + ((this->w - this->min_x)
                            * std::get<PINFO_FRAME>(px)))
                          * sizeof(Uint32));
        *((Uint32*)surface_pixel) = pfmt;
      }

      //continue
      return true;
    });

    SDL_UnlockSurface(surface);

    //convert to a texture
    w = surface->w;
    h = surface->h;

    //use pixels to create texture
    SDL_Texture *texture = SDL_CreateTextureFromSurface(&renderer,surface);

    if(texture == NULL) {
      logger::log_err("failed to create texture: " +
                       std::string(SDL_GetError()));
      exit(1);
    }

    //free surface
    SDL_FreeSurface(surface);
    return texture;
  }

}}
