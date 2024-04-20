/*
    Abstract Hittable material struct
*/

#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdbool.h>

#include "color.h"
#include "common.h"
#include "hittable.h"
#include "ray.h"

// Scatter functions need to conform to this form!
typedef bool (*ScatterFunction)(void *mat_obj, const Ray *r_in,
                                const hit_record *rec, Color *attenuation,
                                Ray *scattered);

typedef struct material {
  void *object;                      // Actual material
  ScatterFunction scatter_function;  // Specialized light scatter function
} Material;

////////////////////////////////////////////
typedef struct lambertian {
  Color albedo;
} Lambertian;

bool lamb_scatter(void *obj, const Ray *r_in, const hit_record *rec,
                  Color *attenuation, Ray *scattered);
Material make_lambertian(Color c);

////////////////////////////////////////////
typedef struct metal {
  Color albedo;
  float fuzziness;
} Metal;

bool metal_scatter(void *obj, const Ray *r_in, const hit_record *rec,
                   Color *attenuation, Ray *scattered);

Material make_metal(Color c, float fuzz);

////////////////////////////////////////////

inline void material_clear(Material *mat) { free(mat->object); }
#endif