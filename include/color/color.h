#ifndef COLOR
#define COLOR

#include "vec3.h"
#include <time.h>

typedef struct color {
  float r, g, b;
} Color;

static const Color WHITE = {1.0, 1.0, 1.0};
static const Color BLUE = {0.5, 0.7, 1.0};
static const Color RED = {1.0, 0.0, 0.0};
static const Color BLACK = {0.0, 0.0, 0.0};

inline Vec3 colToVec(Color c) { return make_vec3(c.r, c.g, c.b); }

inline Color vecToCol(Vec3 v) {
  Color c = {v.e[0], v.e[1], v.e[2]};
  return c;
}

inline float linear_to_gamma(float linear_component) { return sqrt(linear_component); }
inline Color gamma_correct(Color* c) {
  c->r = sqrt(c->r);
  c->g = sqrt(c->g);
  c->b = sqrt(c->b);
}

#ifdef __arm__
void write_color(int x, int y, Color pixel_color, int samples_per_pixel);

#elif __x86_64__
void write_color(FILE* out, Color pixel_color, int samples_per_pixel);
#else
void write_color(void) {}
#endif
#endif