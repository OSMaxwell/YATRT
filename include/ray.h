/*
 * Ray(Line) header-only.
 */

#ifndef RAY_H
#define RAY_H
#include "vec3.h"

typedef struct ray {
  Vec3 origin;
  Vec3 direction;
} Ray;

Ray make_ray(const Vec3 *origin, const Vec3 *direction) {
  Ray r;
  r.origin = make_vec3(origin->e[0], origin->e[1], origin->e[2]);
  r.direction = make_vec3(direction->e[0], direction->e[1], direction->e[2]);
  return r;
}

Vec3 ray_at(const Ray *r, float t) {
  Vec3 scaledDir = vec3_scale_cpy(r->direction, t);
  return vec3_add(&r->origin, &scaledDir);
}

float ray_hit_sphere(Vec3 center, float radius, Ray *r) {
  Vec3 oc = vec3_sub(&r->origin, &center);
  float a = vec3_length_squared(r->direction);
  float half_b = vec3_dot(oc, r->direction);
  float c = vec3_length_squared(oc) - radius * radius;
  float discriminant = half_b * half_b - a * c;

  if (discriminant < 0) {
    return -1.0;
  } else {
    return (-half_b - sqrt(discriminant)) / a;
  }
}

#endif