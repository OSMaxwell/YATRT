#include "common.h"

#include "sphere.h"

bool hit_Sphere(const void *object, const Ray *r, Interval ray_t,
                hit_record *rec) {
  Sphere *sphere = (Sphere *)object;
  Vec3 oc = vec3_sub(&r->origin, &sphere->center);
  float a = vec3_length_squared(r->direction);
  float half_b = vec3_dot(oc, r->direction);
  float c = vec3_length_squared(oc) - sphere->radius * sphere->radius;
  float discriminant = half_b * half_b - a * c;
  if (discriminant < 0) return false;  // no hit

  float sqrtd = sqrt(discriminant);
  // Find the nearest root that lies in the acceptable range.
  float root = (-half_b - sqrtd) / a;
  if (!interval_surrounds(&ray_t, root)) {
    root = (-half_b + sqrtd) / a;
    if (!interval_surrounds(&ray_t, root)) return false;  // out of range
  }

  // Update hit record
  rec->t = root;
  rec->p = ray_at(r, rec->t);
  Vec3 outward_normal = vec3_sub(&(rec->p), &sphere->center);
  outward_normal = vec3_scalar_div_cpy(outward_normal, sphere->radius);
  set_face_normal(rec, r, &outward_normal);
  rec->mat = sphere->mat;
  return true;
}

Hittable make_Sphere(const Vec3 *center, const float radius,
                     Material *material) {
  Sphere *s = malloc(sizeof(Sphere));
  vec3_vec3_assign(center, &s->center);
  s->radius = radius;
  s->mat = material;

  // Create hittable obj and register hit func
  Hittable hittable;
  hittable.object = s;
  hittable.hit_function = hit_Sphere;
  return hittable;
}