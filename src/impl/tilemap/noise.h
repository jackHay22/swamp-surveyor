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

  /**
   * Generate fractral brownian motion noise
   * UPenn CIS460 noise lecture
   */
  float fractal_brownian_motion(float seed,float x);

}}}

#endif /*_IO_JACKHAY_SWAMP_TILEMAP_NOISE_H*/
