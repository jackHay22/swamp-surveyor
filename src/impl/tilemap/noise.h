/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#ifndef _IO_JACKHAY_SWAMP_TILEMAP_NOISE_H
#define _IO_JACKHAY_SWAMP_TILEMAP_NOISE_H

namespace impl {
namespace tilemap {
namespace noise {

  #define FBM_PERSISTENCE_0_75 0.75f
  #define FBM_PERSISTENCE_0_66 0.66f

  /**
   * Generate fractral brownian motion noise
   * UPenn CIS460 noise lecture
   */
  float fractal_brownian_motion(float seed,float x, float p);

}}}

#endif /*_IO_JACKHAY_SWAMP_TILEMAP_NOISE_H*/
