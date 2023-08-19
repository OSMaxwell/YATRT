#ifndef HITTABLE_H
#define HITTABLE_H

#include "common.h"

typedef struct material Material;

typedef struct hit_record {
  Vec3 p;
  Vec3 normal;
  double t;
  bool front_face;
  Material *mat;
} hit_record;

// Hit functions need to conform to this form!
typedef bool (*HitFunction)(const void *object, const Ray *ray, Interval ray_t,
                            hit_record *record);

typedef struct hittable {
  void *object;             // Pointer to the actual object
  HitFunction hit_function; // Function pointer for hit detection
} Hittable;

void set_face_normal(hit_record *record, const Ray *r,
                     const Vec3 *outward_normal) {
  // Sets the hit record normal vector.
  // NOTE: the parameter `outward_normal` is assumed to have unit length.

  record->front_face = vec3_dot(r->direction, *outward_normal) < 0;
  record->normal = record->front_face
                       ? *outward_normal
                       : vec3_scalar_div_cpy(*outward_normal, -1.0);
}

#endif