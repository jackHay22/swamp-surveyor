/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TRANSPARENT_BLOCK_H
#define _IO_JACKHAY_SWAMP_TRANSPARENT_BLOCK_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <memory>

namespace impl {
namespace tilemap {

  /**
   * Defines a tile that is opaque
   * until the player is within the
   * set bounds. If no texture provided,
   * a colored square is drawn
   */
  struct transparent_block_t {
  private:
    //the bounds
    SDL_Rect bounds;

    //the texture (optional)
    SDL_Texture* texture = NULL;

    //the dimensions of the texture (if applicable)
    int texture_w;
    int texture_h;

    //if no texture, color
    int r;
    int g;
    int b;

    //whether the block is currently transparent
    bool transparent;

    /**
     * Check if a bounding box collides with this block
     * @param  other the other bounding box
     * @param max_dim whether to consider texture and bounding box and
     * use max
     * @return       whether the two boxes collide
     */
    bool is_collided(const SDL_Rect& other,
                     bool max_dim=false) const;

  public:
    //constructor for texture
    transparent_block_t(int x, int y, int w, int h,
                        const std::string& texture_path,
                        SDL_Renderer& renderer);
    //constructor for color
    transparent_block_t(int x, int y, int w, int h,
                        int r, int g, int b);
    transparent_block_t(const transparent_block_t&) = delete;
    transparent_block_t& operator=(const transparent_block_t&) = delete;

    /**
     * Free texture if applicable
     */
    ~transparent_block_t();

    /**
     * Update the block. If the player intersects,
     * set transparent
     * @param player the bounding box for the player
     */
    void update(const SDL_Rect& player);

    /**
     * Render this block
     * @param renderer the sdl renderer
     * @param camera the camera
     * @param debug    whether debug mode enabled
     */
    void render(SDL_Renderer& renderer,
                const SDL_Rect& camera,
                bool debug) const;
  };

  /**
   * Load transparent blocks
   * @param blocks   the blocks vec set by the call
   * @param path     the path to the configuration file
   * @param renderer the renderer for loading textures
   * @param base_path the resource directory base path
   */
  void mk_transparent_blocks(std::vector<std::shared_ptr<transparent_block_t>>& blocks,
                             const std::string& path,
                             SDL_Renderer& renderer,
                             const std::string& base_path);
}}

#endif /*_IO_JACKHAY_SWAMP_TRANSPARENT_BLOCK_H*/
