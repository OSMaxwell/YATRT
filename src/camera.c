#include "camera.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "common.h"

// Initializes camera params
void initialize(Camera* camera) {
  // Camera
  // Determine viewport dimensions.
  float focal_length = 1.0;
  float viewport_height = 2.0;
  float viewport_width =
      viewport_height * ((float)(SCREEN_WIDTH) / (float)SCREEN_HEIGHT);
  camera->center = make_vec3(0.0, 0.0, 0.0);

  // Calculate the vectors across the horizontal and down the vertical viewport
  // edges.
  Vec3 horizontal = make_vec3(viewport_width, 0.0, 0.0);
  Vec3 vertical = make_vec3(0.0, -viewport_height, 0.0);

  // Calculate the horizontal and vertical delta vectors from pixel to pixel.
  camera->pixel_delta_u = vec3_scalar_div_cpy(horizontal, SCREEN_WIDTH);
  camera->pixel_delta_v = vec3_scalar_div_cpy(vertical, SCREEN_HEIGHT);

  // Calculate the location of the upper left pixel.
  Vec3 _tmp = make_vec3(0.0, 0.0, focal_length);
  Vec3 viewport_upper_left = vec3_sub(&camera->center, &_tmp);

  _tmp = vec3_scalar_div_cpy(horizontal, 2.0);
  viewport_upper_left = vec3_sub(&viewport_upper_left, &_tmp);

  _tmp = vec3_scalar_div_cpy(vertical, 2.0);
  viewport_upper_left = vec3_sub(&viewport_upper_left, &_tmp);

  Vec3 pixel00_loc = vec3_scale_cpy(
      vec3_add(&camera->pixel_delta_u, &camera->pixel_delta_v), 0.5);
  camera->pixel00_loc = vec3_add(&pixel00_loc, &viewport_upper_left);
}

Color ray_color(const Ray* r, int depth, const Hittable_list* world) {
  hit_record rec;
  if (depth <= 0) {
    return BLACK;
  }

  if (hittable_list_hit(world, r, make_interval(0.0001, INFINITY), &rec)) {
    Ray scattered;
    Color attenuation;
    bool scat_res = rec.mat->scatter_function(rec.mat->object, r, &rec,
                                              &attenuation, &scattered);
    if (scat_res) {
      Color c = ray_color(&scattered, depth - 1, world);
      return vecToCol(vec3_mult(colToVec(attenuation), colToVec(c)));
    }
    return BLACK;
  }

  // Background
  Vec3 unit_direction = vec3_unit_vector(r->direction);
  float a = 0.5 * (unit_direction.e[1] + 1.0);

  Vec3 _c0 = vec3_scale_cpy(colToVec(WHITE), (1.0 - a));
  Vec3 _c1 = vec3_scale_cpy(colToVec(BLUE), a);
  Vec3 res = vec3_add(&_c0, &_c1);
  return vecToCol(res);
}

// Returns a random point in the square surrounding a pixel at the origin.
Vec3 pixel_sample_square(const Camera* camera) {
  float px = -0.5 + random_float();
  float py = -0.5 + random_float();
  Vec3 _tmp = vec3_scale_cpy(camera->pixel_delta_v, py);
  Vec3 _tmp0 = vec3_scale_cpy(camera->pixel_delta_u, px);

  return vec3_add(&_tmp0, &_tmp);
}

// Get a randomly sampled camera ray for the pixel at location i,j.
Ray get_ray(Camera* camera, int i, int j) {
  Vec3 pixel_center = camera->pixel00_loc;
  Vec3 _tmp = vec3_scale_cpy(camera->pixel_delta_u, i);
  pixel_center = vec3_add(&pixel_center, &_tmp);
  _tmp = vec3_scale_cpy(camera->pixel_delta_v, j);
  pixel_center = vec3_add(&pixel_center, &_tmp);

  _tmp = pixel_sample_square(camera);
  Vec3 pixel_sample = vec3_add(&pixel_center, &_tmp);

  Vec3 ray_direction = vec3_sub(&pixel_sample, &camera->center);

  return make_ray(&camera->center, &ray_direction);
}