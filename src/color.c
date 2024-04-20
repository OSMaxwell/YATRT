#include "color.h"

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "ST7735_TFT.h"
#include "interval.h"

#ifdef __arm__
uint16_t rgb_to_rgb565_float(float _r, float _g, float _b) {
  // Convert the float RGB values (0.0 to 1.0) to integers (0 to 255)
  uint8_t r_int = (uint8_t)(_r * 255.0);
  uint8_t g_int = (uint8_t)(_g * 255.0);
  uint8_t b_int = (uint8_t)(_b * 255.0);

  // Mask and shift the components to fit into the 5-6-5 format
  uint16_t r = (r_int >> 3) & 0x1F;  // 5 bits for red (5 most significant bits)
  uint16_t g =
      (g_int >> 2) & 0x3F;  // 6 bits for green (6 most significant bits)
  uint16_t b =
      (b_int >> 3) & 0x1F;  // 5 bits for blue (5 most significant bits)

  // Combine the components to form the RGB 565 color
  return (r << 11) | (g << 5) | b;
}

void write_color(int x, int y, Color pixel_color, int samples_per_pixel) {
  float r = pixel_color.r;
  float g = pixel_color.g;
  float b = pixel_color.b;

  // Divide the color by the number of samples.
  const float scale = 1.0 / (float)samples_per_pixel;
  r *= scale;
  g *= scale;
  b *= scale;

  // Gamma correction
  r = linear_to_gamma(r);
  g = linear_to_gamma(g);
  b = linear_to_gamma(b);

  // Write the translated [0,255] value of each color component.
  const Interval intesity = make_interval(0.000, 0.999);
  uint16_t rgb565_color = rgb_to_rgb565_float(interval_clamp(&intesity, r),
                                              interval_clamp(&intesity, g),
                                              interval_clamp(&intesity, b));

  clock_t now = clock();
  drawPixel(x, y, rgb565_color);
  draw_bench[bench_idx] = clock() - now;
  bench_idx++;
}

#elif __x86_64__
void write_color(FILE* out, Color pixel_color, int samples_per_pixel) {
  float r = pixel_color.r;
  float g = pixel_color.g;
  float b = pixel_color.b;

  // Divide the color by the number of samples.
  const float scale = 1.0 / (float)samples_per_pixel;
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
#else
void write_color(void) {}
#endif
