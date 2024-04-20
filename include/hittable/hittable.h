#ifndef HITTABLE_H
#define HITTABLE_H

#include "interval.h"
#include "ray.h"

typedef struct material Material;

typedef struct hit_record {
  Vec3 p;
  Vec3 normal;
  float t;
  bool front_face;
  Material *mat;
} hit_record;

// Hit functions need to conform to this form!
typedef bool (*HitFunction)(const void *object, const Ray *ray, Interval ray_t,
                            hit_record *record);

// A hittable object struct containing a ptr to the object itself and the hit
// function required
typedef struct hittable {
  void *object;              // Pointer to the actual object
  HitFunction hit_function;  // Function pointer for specialized hit detection
} Hittable;

// Sets the hit record normal vector.
void set_face_normal(hit_record *record, const Ray *r,
                     const Vec3 *outward_normal);

#endif