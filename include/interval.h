#ifndef INTERVAL_H
#define INTERVAL_H
#include "math.h"
#include <stdbool.h>

typedef struct interval {
  double min;
  double max;
} Interval;

Interval make_interval(double min, double max) {
  Interval interval;
  interval.min = min;
  interval.max = max;
  return interval;
}

bool interval_contains(Interval *interval, double x) {
  return interval->min <= x && x <= interval->max;
}

bool interval_surrounds(Interval *interval, double x) {
  return interval->min <= x && x <= interval->max;
}

double interval_clamp(const Interval *interval, double x) {
  if (x < interval->min)
    return interval->min;
  if (x > interval->max)
    return interval->max;
  return x;
}

const Interval empty = {INFINITY, -INFINITY};
const Interval universe = {-INFINITY, INFINITY};

#endif