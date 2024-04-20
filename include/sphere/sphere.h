/*
 * Sphere hittable scene object
 */

#ifndef SPHERE_H
#define SHPERE_H

#include "hittable.h"

typedef struct sphere {
  Vec3 center;
  float radius;
  Material *mat;
} Sphere;

bool hit_Sphere(const void *object, const Ray *r, Interval ray_t,
                hit_record *rec);
Hittable make_Sphere(const Vec3 *center, const float radius,
                     Material *material);

#endif