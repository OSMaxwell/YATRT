#ifndef VEC2_H
#define VEC2_H

#include <math.h>
typedef struct vec2 {
  float e[2];
} vec2;


vec2 make_vec2(float e0, float e1) {
  vec2 v;
  v.e[0] = e0;
  v.e[1] = e1;
  return v;
}

vec2 make_vec2_zero() {
  return make_vec2(0.0, 0.0);
}

vec2 vec2_negate(vec2 v) {
  vec2 result;
  result.e[0] = -v.e[0];
  result.e[1] = -v.e[1];
  return result;
}

float vec2_get(vec2 v, int i) {
  return v.e[i];
}

void vec2_set(vec2* v, int i, float val) {
  v->e[i] = val;
}

vec2 vec2_add(vec2 v1, vec2 v2) {
  v1.e[0] += v2.e[0];
  v1.e[1] += v2.e[1];
  return v1;
}

vec2 vec2_scale(vec2 v, float t) {
  v.e[0] *= t;
  v.e[1] *= t;
  return v;
}

vec2 vec2_divide(vec2 v, float t) {
  return vec2_scale(v, 1.0 / t);
}

float vec2_length_squared(vec2 v) {
  return v.e[0] * v.e[0] + v.e[1] * v.e[1];
}

float vec2_length(vec2 v) {
  return sqrt(vec2_length_squared(v));
}
#endif