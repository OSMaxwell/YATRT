#ifndef COMMON_H
#define COMMON_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#ifdef USE_PICO_RAND
#include "pico/rand.h"
#endif
#include <stdlib.h>

extern const float pi;

// Utility Functions

inline float degrees_to_radians(float degrees) { return degrees * pi / 180.0; }

inline float random_float() {
  // Returns a random real in [0,1).
#ifdef USE_PICO_RAND
  int rnd = get_rand_32();
#else
  int rnd = rand();
#endif
  return (float)rnd / (RAND_MAX + 1.0);
}

inline float random_float_interval(float min, float max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_float();
}

#endif