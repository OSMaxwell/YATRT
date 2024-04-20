#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
typedef struct camera {
  float aspect_ratio;
  int samples_per_pixel;  // Count of random samples for each pixel
  int max_depth;          // Maximum number of ray bounces into scene

  Vec3 center;         // Camera center
  Vec3 pixel00_loc;    // Location of pixel 0, 0
  Vec3 pixel_delta_u;  // Offset to pixel to the right
  Vec3 pixel_delta_v;  // Offset to pixel below
} Camera;

// Initializes camera params
void initialize(Camera* camera);
Color ray_color(const Ray* r, int depth, const Hittable_list* world);
// Get a randomly sampled camera ray for the pixel at location i,j.
Ray get_ray(Camera* camera, int i, int j);
void render(const Hittable_list* world, Camera* camera);

#endif