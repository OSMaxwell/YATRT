#ifndef COLOR
#define COLOR
#include "vec3.h"
#include <stdint.h>
#include <stdio.h>

typedef struct color {
  double r, g, b;
} Color;

const Color WHITE = {1.0, 1.0, 1.0};
const Color BLUE = {0.5, 0.7, 1.0};
const Color RED = {1.0, 0.0, 0.0};
const Color BLACK = {0.0, 0.0, 0.0};

Vec3 colToVec(Color c) { return make_vec3(c.r, c.g, c.b); }

Color vecToCol(Vec3 v) {
  Color c = {v.e[0], v.e[1], v.e[2]};
  return c;
}

double linear_to_gamma(double linear_component) {
  return sqrt(linear_component);
}

void write_color(FILE *out, Color pixel_color, int samples_per_pixel) {
  double r = pixel_color.r;
  double g = pixel_color.g;
  double b = pixel_color.b;

  // Divide the color by the number of samples.
  const double scale = 1.0 / (double)samples_per_pixel;
  r *= scale;
  g *= scale;
  b *= scale;

  // Gamma correction
  r = linear_to_gamma(r);
  g = linear_to_gamma(g);
  b = linear_to_gamma(b);

  // Write the translated [0,255] value of each color component.
  const Interval intesity = make_interval(0.000, 0.999);

  // Write the translated [0,255] value of each color component.
  fprintf(out, "%d %d %d\n", (int)(256 * interval_clamp(&intesity, r)),
          (int)(256 * interval_clamp(&intesity, g)),
          (int)(256 * interval_clamp(&intesity, b)));
}

uint16_t rgb_to_rgb565_double(Color c) {
  // Convert the double RGB values (0.0 to 1.0) to integers (0 to 255)
  uint8_t r_int = (uint8_t)(c.r * 255.0);
  uint8_t g_int = (uint8_t)(c.g * 255.0);
  uint8_t b_int = (uint8_t)(c.b * 255.0);

  // Mask and shift the components to fit into the 5-6-5 format
  uint16_t r = (r_int >> 3) & 0x1F; // 5 bits for red (5 most significant bits)
  uint16_t g =
      (g_int >> 2) & 0x3F; // 6 bits for green (6 most significant bits)
  uint16_t b = (b_int >> 3) & 0x1F; // 5 bits for blue (5 most significant bits)

  // Combine the components to form the RGB 565 color
  return (r << 11) | (g << 5) | b;
}

#endif