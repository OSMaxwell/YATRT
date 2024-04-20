/*
 * Ray(Line)
 */

#ifndef RAY_H
#define RAY_H
#include "vec3.h"

typedef struct ray {
  Vec3 origin;
  Vec3 direction;
} Ray;

Ray make_ray(const Vec3 *origin, const Vec3 *direction);
Vec3 ray_at(const Ray *r, float t);
float ray_hit_sphere(Vec3 center, float radius, Ray *r);

#endif