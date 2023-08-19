/*
 * Vector_3 header-only. This also inclues tests for given funcs
 */
#ifndef VEC3_H
#define VEC3_H
#include "common.h"
#include <math.h>

typedef struct vec3 {
  double e[3];
} Vec3;

#define _precision 1e-6

#define VEC3_EQUAL(v1, v2)                                                     \
  ((v1.e[0] == v2.e[0]) && (v1.e[1] == v2.e[1]) && (v1.e[2] == v2.e[2]))

#define VEC3_EQUAL_ABS(v1, v2)                                                 \
  ((fabs((v1.e[0]) - (v2.e[0])) < _precision) &&                               \
   (fabs((v1.e[1]) - (v2.e[1])) < _precision) &&                               \
   (fabs((v1.e[2]) - (v2.e[2])) < _precision))

Vec3 make_vec3(double e0, double e1, double e2) {
  Vec3 v;
  v.e[0] = e0;
  v.e[1] = e1;
  v.e[2] = e2;
  return v;
}

Vec3 make_vec3_zero() { return make_vec3(0.0, 0.0, 0.0); }

double vec3_get(Vec3 *v, int i) { return v->e[i]; }

void vec3_set(Vec3 *v, int i, double val) { v->e[i] = val; }

void vec3_assign(Vec3 *v, double e0, double e1, double e2) {
  v->e[0] = e0;
  v->e[1] = e1;
  v->e[2] = e2;
}

//  u <- v
void vec3_vec3_assign(const Vec3 *v, Vec3 *u) {
  vec3_assign(u, v->e[0], v->e[1], v->e[2]);
}

void vec3_negate(Vec3 *v) {
  v->e[0] = -v->e[0];
  v->e[1] = -v->e[1];
  v->e[2] = -v->e[2];
}

// Return v1+v2
Vec3 vec3_add(const Vec3 *v1, const Vec3 *v2) {
  Vec3 result;
  result.e[0] = v1->e[0] + v2->e[0];
  result.e[1] = v1->e[1] + v2->e[1];
  result.e[2] = v1->e[2] + v2->e[2];
  return result;
}

// Return u-v
Vec3 vec3_sub(const Vec3 *u, const Vec3 *v) {
  Vec3 result;
  result.e[0] = u->e[0] - v->e[0];
  result.e[1] = u->e[1] - v->e[1];
  result.e[2] = u->e[2] - v->e[2];
  return result;
}

Vec3 vec3_sub_scalar(const Vec3 *u, double t) {
  Vec3 result;
  result.e[0] = u->e[0] - t;
  result.e[1] = u->e[1] - t;
  result.e[2] = u->e[2] - t;
  return result;
}

// Inplace scale (=*)
void vec3_scale(Vec3 *v, double t) {
  v->e[0] *= t;
  v->e[1] *= t;
  v->e[2] *= t;
}

Vec3 vec3_scale_cpy(Vec3 v, double t) {
  Vec3 result;
  result.e[0] = v.e[0];
  result.e[1] = v.e[1];
  result.e[2] = v.e[2];
  vec3_scale(&result, t);
  return result;
}

// Inplace div (=/)
void vec3_scalar_div(Vec3 *v, double t) {
  v->e[0] = v->e[0] / t;
  v->e[1] = v->e[1] / t;
  v->e[2] = v->e[2] / t;
}

Vec3 vec3_scalar_div_cpy(Vec3 v, double t) {
  Vec3 result;
  result.e[0] = v.e[0];
  result.e[1] = v.e[1];
  result.e[2] = v.e[2];
  vec3_scalar_div(&result, t);
  return result;
}

double vec3_length_squared(Vec3 v) {
  return v.e[0] * v.e[0] + v.e[1] * v.e[1] + v.e[2] * v.e[2];
}

double vec3_length(Vec3 v) { return sqrt(vec3_length_squared(v)); }

Vec3 vec3_mult(Vec3 u, Vec3 v) {
  Vec3 result;
  result.e[0] = u.e[0] * v.e[0];
  result.e[1] = u.e[1] * v.e[1];
  result.e[2] = u.e[2] * v.e[2];
  return result;
}

Vec3 vec3_unit_vector(Vec3 v) {
  double len = vec3_length(v);
  vec3_scalar_div(&v, len);
  return v;
}

double vec3_dot(Vec3 u, Vec3 v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

Vec3 vec3_cross(Vec3 *u, Vec3 *v) {
  Vec3 result;
  result.e[0] = u->e[1] * v->e[2] - u->e[2] * v->e[1];
  result.e[1] = u->e[2] * v->e[0] - u->e[0] * v->e[2];
  result.e[2] = u->e[0] * v->e[1] - u->e[1] * v->e[0];
  return result;
}

// Diffuse funcs

Vec3 vec3_random() {
  return make_vec3(random_double(), random_double(), random_double());
}

Vec3 vec3_random_interval(double min, double max) {
  return make_vec3(random_double_interval(min, max),
                   random_double_interval(min, max),
                   random_double_interval(min, max));
}

Vec3 random_in_unit_sphere() {
  while (1) {
    Vec3 p = vec3_random_interval(-1.0, 1.0);
    if (vec3_length_squared(p) < 1)
      return p;
  }
}
Vec3 random_unit_vector() { return vec3_unit_vector(random_in_unit_sphere()); }

Vec3 random_on_hemisphere(const Vec3 *normal) {
  Vec3 on_unit_sphere = random_unit_vector();
  if (vec3_dot(on_unit_sphere, *normal) < 0.0) {
    // Not in the same hemisphere as the normal
    vec3_negate(&on_unit_sphere);
  }
  return on_unit_sphere;
}

bool vec3_near_zero(Vec3 v) {
  // Return true if the vector is close to zero in all dimensions.
  return (fabs(v.e[0]) < _precision) && (fabs(v.e[1]) < _precision) &&
         (fabs(v.e[2]) < _precision);
}

Vec3 reflect(const Vec3 *v, const Vec3 *n) {
  double _dot = vec3_dot(*v, *n);
  Vec3 _tmp = vec3_scale_cpy(*n, 2 * _dot);
  return vec3_sub(v, &_tmp);
}

// Vec3 testSuite
int testcases() {
  // Test cases for make_vec3 and vec3_get
  Vec3 v1 = make_vec3(1.0, 2.0, 3.0);
  if (vec3_get(&v1, 0) != 1.0) {
    return 1;
  }
  if (vec3_get(&v1, 1) != 2.0) {
    return 1;
  }
  if (vec3_get(&v1, 2) != 3.0) {
    return 1;
  }

  // Test cases for vec3_add
  Vec3 v2 = make_vec3(0.5, 0.5, 0.5);
  Vec3 sum = vec3_add(&v1, &v2);
  Vec3 res = {.e = {1.5, 2.5, 3.5}};
  if (!VEC3_EQUAL(sum, res))
    return 1;

  // Test cases for vec3_sub
  Vec3 diff = vec3_sub(&v1, &v2);
  vec3_assign(&res, 0.5, 1.5, 2.5);
  if (!VEC3_EQUAL(diff, res))
    return 1;

  // Test cases for vec3_scale and vec3_scale_cpy
  Vec3 scaled_v0;
  vec3_vec3_assign(&v1, &scaled_v0);
  vec3_scale(&scaled_v0, 2.0);
  Vec3 scaled_v1 = vec3_scale_cpy(v1, 2.0);
  if (!VEC3_EQUAL(scaled_v1, scaled_v0))
    return 1;

  vec3_assign(&res, 2.0, 4.0, 6.0);
  if (!VEC3_EQUAL(res, scaled_v0))
    return 1;

  // Test cases for vec3_length
  double length = vec3_length(v1);
  if ((fabs(length) - 3.74166 > _precision))
    return 1;

  // Test cases for vec3_unit_vector
  Vec3 unit_v2 = vec3_unit_vector(v2);
  vec3_assign(&res, 0.577350, 0.577350, 0.577350);
  if (!VEC3_EQUAL_ABS(res, unit_v2))
    return 1;

  // Test cases for vec3_dot
  double dot_product = vec3_dot(v1, v2);
  if (dot_product != 3.0)
    return 1;

  // Test cases for vec3_cross
  Vec3 cross_product = vec3_cross(&v1, &v2);
  vec3_assign(&res, -0.500000, 1.000000, -0.500000);
  if (!VEC3_EQUAL(res, cross_product))
    return 1;

  return 0;
}

#endif