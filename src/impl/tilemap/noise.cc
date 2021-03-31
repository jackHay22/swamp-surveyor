/*
 * (C) 2021 Jack Hay
 *
 * Untitled Swamp game
 */

#include "noise.h"
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <cmath>

namespace impl {
namespace tilemap {
namespace noise {

  /**
   * Get the fractional part of x
   */
  float fract(float x) {
    return x - floor(x);
  }

  /**
   * Dot product
   */
  float dot(float x1, float y1, float x2, float y2) {
    return (x1 * x2) + (y1 * y2);
  }

  /**
   * Linear interpolate of x,y based on a
   */
  float interp(float x, float y, float a) {
    return x * (1.0 - a) + y * a;
  }

  /**
   * 1 dimensional noise
   * UPenn CIS460 noise lecture
   */
  float noise(float seed, float x) {
    return fract(sin(dot(x, seed, 127.1, 311.7)) * 43758.5453);
  }

  /**
   * Interpolate 1D noise
   * UPenn CIS460 noise lecture
   */
  float interpolate_noise(float seed,float x) {
    int xi = int(floor(x));
    return interp(
      noise(seed,xi),
      noise(seed,xi+1),
      fract(x)
    );
  }

  /**
   * Generate fractral brownian motion noise
   * UPenn CIS460 noise lecture
   */
  float fractal_brownian_motion(float seed, float x) {
    float total = 0.f;
    float persistence = 0.75f;
    int octaves = 8;

    for(int i = 1; i <= octaves; i++) {
      float freq = pow(2.f, i);
      float amp = pow(persistence, i);

      total += interpolate_noise(seed,x * freq) * amp;
    }
    return total;
  }

}}}
