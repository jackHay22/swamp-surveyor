/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TEXTURE_CONSTRUCTOR_H
#define _IO_JACKHAY_SWAMP_TEXTURE_CONSTRUCTOR_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <tuple>
#include <map>
#include <vector>
#include <memory>
#include <limits.h>
#include <functional>

namespace impl {
namespace environment {

  //position in map type
  typedef std::pair<int,int> pos_t;

  //pixel information type
  typedef std::tuple<pos_t,int,std::tuple<Uint8,Uint8,Uint8>> px_t;

  //an animation frame
  typedef std::map<pos_t,px_t> frame_t;

  #define PINFO_POS 0
  #define PINFO_FRAME 1
  #define PINFO_RGB 2

  /**
   * Create an SDL texture from a map of rgb pixels
   */
  struct texture_constructor_t {
  private:
    //calculated dimension
    int w;
    int h;
    //min y value seen
    int min_y;
    //min x value seen
    int min_x;

    //default colors
    int r;
    int g;
    int b;
    //the rgb values of pixels in this texture
    //Format: (x,y) -> (x,y,frame,(r,g,b))
    std::vector<std::unique_ptr<frame_t>> frame_sets;

    /**
     * Execute some function on each pixel
     * If the function returns false, we exit the loop
     * @param fn the function
     */
    void for_each(std::function<bool(px_t&)> fn);

    /**
     * Execute some function on each pixel
     * If the function returns false, we exit the loop
     * @param fn the function
     */
    void for_each(std::function<bool(const px_t&)> fn) const;

    /**
     * Get the frame for a given index (create if not yet set)
     * @param  frame frame index
     * @return       the frame
     */
    frame_t& get_frame(size_t frame);

  public:
    /**
     * Constructor
     */
    texture_constructor_t();
    texture_constructor_t(const texture_constructor_t&) = delete;
    texture_constructor_t& operator=(const texture_constructor_t&) = delete;

    /**
     * Clamp the bounds of the texture based on pixels added so far
     * @param buffer a buffer to add around the existing pixels
     */
    void clamp(int buffer=0);

    /**
     * Get the current width of the texture
     * @return the width of the texture so far
     */
    int get_width() const { return (w - min_x) / get_frames(); }

    /**
     * Get the height of the texture
     * @return the height of the texture
     */
    int get_height() const { return h - min_y; }

    /**
     * Get the number of frames in this texture (set when adding pixels)
     * @return the number of frames (>= 1)
     */
    int get_frames() const { return std::max(1,(int)frame_sets.size()); }

    /**
     * Check if a given position has a texture already
     * @param  x pos x
     * @param  y pos y
     * @return   whether a pixel is set at this position already
     */
    bool is_set(int x, int y) const;

    /**
     * Set the default color to use
     * @param r r channel
     * @param g g channel
     * @param b b channel
     */
    void set_default_color(Uint8 r,Uint8 g,Uint8 b);

    /**
     * Set default color at some position
     * @param x position x
     * @param y position y
     * @param frame the frame to set the pixel in
     *        (if -1, set in ALL)
     */
    void set(int x, int y, int frame=-1);

    /**
     * Set a rectangle, takes position, dimension and frame
     */
    void set_rect(int x, int y, int w, int h, int frame=-1);

    /**
     * Erase the pixel at a given position (if exists)
     */
    void erase(int x, int y, int frame=-1);

    /**
     * Render a line between two points
     */
    void set_line(int x1, int y1, int x2, int y2, int thickness, int frame=-1);

    /**
     * Set the color of a pixel at some position
     * @param x the position in the texture x
     * @param y the position in the texture y
     * @param r color of the pixel r
     * @param g color of the pixel g
     * @param b color of the pixel b
     * @param frame the frame to set this pixel in
     *              (if -1, set in ALL)
     */
    void set(int x, int y, Uint8 r, Uint8 g, Uint8 b, int frame=-1);

    /**
     * Generate the texture
     * @param  renderer the renderer for loading the texture
     * @param  w        the width of the texture set by the call
     * @param  h        the height of the texture set by the call
     * @return the      texture
     */
    SDL_Texture* generate(SDL_Renderer& renderer, int& w, int& h) const;

  };
}}

#endif /*_IO_JACKHAY_SWAMP_TEXTURE_CONSTRUCTOR_H*/
