/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_ENVIRONMENT_CROWS_H
#define _IO_JACKHAY_SWAMP_ENVIRONMENT_CROWS_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include "renderable.h"
#include "single_seq_anim.h"
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include "../entity/anim_set.h"

namespace impl {
namespace environment {

  /**
   * A random crow generator
   */
  struct crows_t : public renderable_t {
  private:
    //crow positions and directions
    std::vector<std::tuple<float,float,bool>> crows;

    //the crow animation
    std::unique_ptr<entity::anim_set_t> anim;

    //point in map to the right of which crows moving left are created
    int width;

    /**
     * Create a new crow
     * @param whether this is the initial setup phase
     * @return the crow position and direction
     */
    std::tuple<int,int,bool> new_crow(bool initial);

    /**
     * Check if a given point lies within the rectangle provided
     * @param crow the bounds of the crow to check
     * @param camera the rectangle
     * @return whether the point is in the rectangle
     */
    bool in_camera(const SDL_Rect& crow, const SDL_Rect& camera) const;

  public:
    /**
     * Crows constructor
     * @param count        the number of crows in the map at all times
     * @param width        the width of the map (crows moving left start beyond this point)
     * @param anim_path    the path to the tree texture
     * @param base_path    the path to the resources folder
     * @param renderer the renderer for loading the texture
     * @param total_frames the number of animation frames
     */
    crows_t(int count,
            int width,
            const std::string& anim_path,
            const std::string& base_path,
            SDL_Renderer& renderer);
    crows_t(const crows_t&) = delete;
    crows_t& operator=(const crows_t&) = delete;

    /**
     * Update the crows
     */
    void update();

    /**
     * Render the crows
     * @param renderer the renderer to use
     * @param camera   the camera
     * @param debug    whether debug mode enabled
     */
    void render(SDL_Renderer& renderer,
                const SDL_Rect& camera,
                bool debug) const;

  };
}}

#endif /*_IO_JACKHAY_SWAMP_ENVIRONMENT_CROWS_H*/
