/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_MAP_FORK_H
#define _IO_JACKHAY_SWAMP_MAP_FORK_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include "../state/state_manager.h"

namespace impl {
namespace misc {

  struct map_fork_t;

  /**
   * Load map forks
   * @param forks    forks loaded by the call
   * @param cfg_path path to the fork configuration file
   * @pararm font_path the path to the font to use
   * @param renderer the sdl renderer for loading textures
   */
  void load_forks(std::vector<std::shared_ptr<map_fork_t>>& forks,
                  const std::string& cfg_path,
                  const std::string& font_path,
                  SDL_Renderer& renderer);

  /**
   * Defines a fork in the tilemap that can be
   * followed
   */
  struct map_fork_t {
  private:
    //position in the map
    int x;
    int y;

    //target position
    int target_x;
    int target_y;

    //texture for the fork message
    SDL_Texture *texture;
    int texture_w;
    int texture_h;

    //destination
    state::state_type dest;

    //whether to display the message
    bool display;

    /**
     * Check if a bounding box collides with this fork location
     * @param  bounds the bounds to check
     * @return        whether the bounds collide
     */
    bool is_collided(const SDL_Rect& bounds) const;

  public:
    /**
     * Constructor
     * @param x           fork position x
     * @param y           fork position y
     * @param target_x    target position x (dest map)
     * @param target_y    target position y
     * @param target_text the name to display to player
     * @param target_name the name of the target state
     * @param font_path   path to the font to load
     * @param renderer    renderer for loading the font
     */
    map_fork_t(int x, int y,
               int target_x, int target_y,
               const std::string& target_text,
               const std::string& target_name,
               const std::string& font_path,
               SDL_Renderer& renderer);
    map_fork_t(const map_fork_t&) = delete;
    map_fork_t& operator=(const map_fork_t&) = delete;

    /**
     * Free texture
     */
    ~map_fork_t();

    /**
     * Get the target position
     * @param tx position x
     * @param ty position y
     */
    void get_target(int& tx, int& ty) const { tx = target_x; ty = target_y; }

    /**
     * Whether the fork can be taken
     * @return whether the player is in range
     */
    bool can_interact() { return display; }

    /**
     * Get the destination
     * @return dest
     */
    state::state_type get_dest() { return dest; };

    /**
     * Update the fork message
     * @param player the position of the player
     */
    void update(const SDL_Rect& player);

    /**
     * Render the message about this fork
     * @param renderer the renderer to use
     * @param camera   the camera
     */
    void render(SDL_Renderer& renderer,
                const SDL_Rect& camera) const;

  };
}}

#endif /*_IO_JACKHAY_SWAMP_MAP_FORK_H*/
