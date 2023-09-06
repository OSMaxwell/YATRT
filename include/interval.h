#ifndef INTERVAL_H
#define INTERVAL_H
#include <stdbool.h>

#include "math.h"

typedef struct interval {
  float min;
  float max;
} Interval;

Interval make_interval(float min, float max) {
  Interval interval;
  interval.min = min;
  interval.max = max;
  return interval;
}

bool interval_contains(Interval *interval, float x) {
  return interval->min <= x && x <= interval->max;
}

bool interval_surrounds(Interval *interval, float x) {
  return interval->min <= x && x <= interval->max;
}

float interval_clamp(const Interval *interval, float x) {
  if (x < interval->min) return interval->min;
  if (x > interval->max) return interval->max;
  return x;
}

const Interval empty = {INFINITY, -INFINITY};
const Interval universe = {-INFINITY, INFINITY};

#endif