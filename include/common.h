#ifndef COMMON_H
#define COMMON_H

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

const double infinity = INFINITY;
const double pi = 3.141592653589;

// Utility Functions

inline double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}

double random_double() {
  // Returns a random real in [0,1).
  return rand() / (RAND_MAX + 1.0);
}

double random_double_interval(double min, double max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_double();
}

// Common Headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif