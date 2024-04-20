#include "material.h"

////////////////////////////////////////////
bool lamb_scatter(void *obj, const Ray *r_in, const hit_record *rec,
                  Color *attenuation, Ray *scattered) {
  // Identify obj
  Lambertian *lambert = (Lambertian *)obj;

  Vec3 rnd = random_unit_vector();
  Vec3 scatter_direction = vec3_add(&rec->normal, &rnd);

  // Catch degenerate scatter direction
  if (vec3_near_zero(scatter_direction)) {
    scatter_direction = rec->normal;
  }

  *scattered = make_ray(&rec->p, &scatter_direction);
  *attenuation = lambert->albedo;
  return true;
}

Material make_lambertian(Color c) {
  Lambertian *l = (Lambertian *)malloc(sizeof(Lambertian));
  l->albedo.r = c.r;
  l->albedo.g = c.g;
  l->albedo.b = c.b;

  // Create material and register func
  Material mat;
  mat.object = l;
  mat.scatter_function = lamb_scatter;
  return mat;
}
////////////////////////////////////////////

bool metal_scatter(void *obj, const Ray *r_in, const hit_record *rec,
                   Color *attenuation, Ray *scattered) {
  // Identify obj
  Metal *metal = (Metal *)obj;
  Vec3 unit = vec3_unit_vector(r_in->direction);
  Vec3 reflected = reflect(&unit, &rec->normal);
  unit = random_unit_vector();
  vec3_scale(&unit, metal->fuzziness);
  reflected = vec3_add(&reflected, &unit);

  *scattered = make_ray(&rec->p, &reflected);
  *attenuation = metal->albedo;
  float res = vec3_dot(scattered->direction, rec->normal);
  return (bool)(res > 0.0);
}

Material make_metal(Color c, float fuzz) {
  Metal *m = (Metal *)malloc(sizeof(Metal));
  m->albedo.r = c.r;
  m->albedo.g = c.g;
  m->albedo.b = c.b;
  m->fuzziness = fuzz;

  // Create material and register func
  Material mat;
  mat.object = m;
  mat.scatter_function = metal_scatter;
  return mat;
}

////////////////////////////////////////////