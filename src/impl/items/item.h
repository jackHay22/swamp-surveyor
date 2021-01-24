/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ITEM_H
#define _IO_JACKHAY_SWAMP_ITEM_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <string>
#include "../tilemap/tilemap.h"

namespace impl {
namespace items {

  /**
   * An item in the map
   * that can be picked up by the
   * player
   */
  struct item_t {
  private:
    //the location of the item in the map
    int x;
    int y;

    //the dimensions of the texture
    int texture_w;
    int texture_h;

    //the ticks since the last position update
    int position_ticks;
    //whether the texture bounce is currently up
    bool position_up;

    //whether this item has been picked up
    bool picked_up;
    //once picked up, the position the item moves to
    int target_x;
    int target_y;

    //whether this item can now be shown in inventory
    bool displayable;
    //once displayed the coordinates (not transformed by camera)
    int display_x;
    int display_y;

    //the texture
    SDL_Texture* texture = NULL;

  public:
    /**
     * Constructor
     * @param x            position of the item x
     * @param y            position of the item y
     * @param texture_path the path to the texture
     * @param renderer     the renderer for loading the texture
     */
    item_t(int x, int y,
           const std::string& texture_path,
           SDL_Renderer& renderer);
    item_t(const item_t&) = delete;
    item_t& operator=(const item_t&) = delete;

    /**
     * Free texture
     */
    ~item_t();

    /**
     * Whether this item can be removed from the environment
     * @return whether this item has finished any final animations
     */
    bool removable() const { return displayable; }

    /**
     * Set the location for displaying this item
     * @param display_x the display coordinate x
     * @param display_y the display coordinate y
     */
    void set_display_position(int display_x, int display_y);

    /**
     * Player picks up this item, animate movement
     * @param x the player position x
     * @param y the player position y
     */
    void pick_up(int x, int y);

    /**
     * Drop this inventory item
     * @param x the position x
     * @param y the position y
     */
    void drop(int x, int y);

    /**
     * Check if a bounding box collides with this item
     * @param  bounds the bounds to check
     * @return        whether the bounds collide
     */
    bool is_collided(const SDL_Rect& bounds) const;

    /**
     * Get the bounding box for this item
     * @return the bounding box for this item
     */
    SDL_Rect get_bounds() const;

    /**
     * Update the item
     * @param map the tilemap
     */
    virtual void update(const tilemap::tilemap_t& map);

    /**
     * Render the item
     * @param renderer the renderer to use
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    virtual void render(SDL_Renderer& renderer,
                        const SDL_Rect& camera,
                        bool debug) const;
  };
}}

#endif /*_IO_JACKHAY_SWAMP_ITEM_H*/
