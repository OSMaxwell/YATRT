/*
 * Vector_3 header-only. This also inclues tests for given funcs
 */
#ifndef VEC3_H
#define VEC3_H
#include <math.h>

#include "common.h"

typedef struct vec3 {
  float e[3];
} Vec3;

#define _precision 1e-6

#define VEC3_EQUAL(v1, v2) \
  ((v1.e[0] == v2.e[0]) && (v1.e[1] == v2.e[1]) && (v1.e[2] == v2.e[2]))

#define VEC3_EQUAL_ABS(v1, v2)                   \
  ((fabs((v1.e[0]) - (v2.e[0])) < _precision) && \
   (fabs((v1.e[1]) - (v2.e[1])) < _precision) && \
   (fabs((v1.e[2]) - (v2.e[2])) < _precision))

Vec3 make_vec3(float e0, float e1, float e2);
Vec3 make_vec3_zero();
float vec3_get(Vec3 *v, int i);
void vec3_set(Vec3 *v, int i, float val);
void vec3_assign(Vec3 *v, float e0, float e1, float e2);
//  u <- v
void vec3_vec3_assign(const Vec3 *v, Vec3 *u);
void vec3_negate(Vec3 *v);
// Return v1+v2
Vec3 vec3_add(const Vec3 *v1, const Vec3 *v2);
// Return u-v
Vec3 vec3_sub(const Vec3 *u, const Vec3 *v);
Vec3 vec3_sub_scalar(const Vec3 *u, float t);
// Inplace scale (=*)
void vec3_scale(Vec3 *v, float t);
Vec3 vec3_scale_cpy(Vec3 v, float t);
// Inplace div (=/)
void vec3_scalar_div(Vec3 *v, float t);
Vec3 vec3_scalar_div_cpy(Vec3 v, float t);
float vec3_length_squared(Vec3 v);
float vec3_length(Vec3 v);
Vec3 vec3_mult(Vec3 u, Vec3 v);
Vec3 vec3_unit_vector(Vec3 v);
float vec3_dot(Vec3 u, Vec3 v);
Vec3 vec3_cross(Vec3 *u, Vec3 *v);

// Diffuse func

Vec3 vec3_random();
Vec3 vec3_random_interval(float min, float max);
Vec3 random_in_unit_sphere();
Vec3 random_unit_vector();
Vec3 random_on_hemisphere(const Vec3 *normal);
bool vec3_near_zero(Vec3 v);
Vec3 reflect(const Vec3 *v, const Vec3 *n);

#ifdef TESTSUITE
// Vec3 testSuite
int testcases();
#endif
#endif