#include "hittable.h"

#include "common.h"
#include "interval.h"
#include "ray.h"

// Sets the hit record normal vector.
void set_face_normal(hit_record *record, const Ray *r,
                     const Vec3 *outward_normal) {
  // NOTE: the parameter `outward_normal` is assumed to have unit length.

  record->front_face = vec3_dot(r->direction, *outward_normal) < 0;
  record->normal = record->front_face
                       ? *outward_normal
                       : vec3_scalar_div_cpy(*outward_normal, -1.0);
}