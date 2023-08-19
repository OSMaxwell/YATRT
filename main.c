// #include <Adafruit_GFX.h>
// #include <Adafruit_ST7735.h>

#include "camera.h"
#include "color.h"
#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include <stdio.h>

#define TFT_CS 10
#define TFT_RST 9
#define TFT_DC 8

// Screen tft global context
// Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// inline void setupScreen() {
//   tft.initR(INITR_BLACKTAB);
//   tft.fillScreen(ST7735_BLACK);
//   tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
//   tft.setTextSize(1);
//   //tft.setRotation(1);
// }

// void setup(void) {
//   setupScreen();
//   Serial.begin(9600);
// }

double progress = 0.0;

// inline void printProgress() {
//   progress += 1.0 / (IMAGE_WIDTH * IMAGE_HEIGHT);
//   tft.setCursor(140, 10);
//   char buffer[5];
//   dtostrf(progress, 4, 2, buffer);  // 4 = total width, 2 = number of
//   decimal places tft.println(buffer);
// }

int main(void) {

  if (testcases() != 0) {
    printf("FAIL!\n");
    return 1;
  }

  printf("PASS\n");
  // TODO: HW Screen Init here

  // World
  // Material
  Material material_ground = make_lambertian((Color){0.8, 0.8, 0.0});
  Material material_center = make_lambertian((Color){0.7, 0.3, 0.3});
  Material material_left = make_metal((Color){0.5, 0.5, 0.75},0.3);
  Material material_right = make_metal((Color){0.8, 0.6, 0.2},1.0);

  // Geometry
  Hittable_list world;
  hittable_list_init(&world);

  Vec3 sphere_center = make_vec3(0.0, 0.0, -1.0);
  Hittable sphere0 = make_Sphere(&sphere_center, 0.5, &material_center);
  hittable_list_add(&world, &sphere0);

  sphere_center = make_vec3(0.0, -100.5, -1.0);
  Hittable sphere1 = make_Sphere(&sphere_center, 100, &material_ground);
  hittable_list_add(&world, &sphere1);

  sphere_center = make_vec3(-1.0, 0.0, -1.0);
  Hittable sphere2 = make_Sphere(&sphere_center, 0.5, &material_left);
  hittable_list_add(&world, &sphere2);

  sphere_center = make_vec3(1.0, 0.0, -1.0);
  Hittable sphere3 = make_Sphere(&sphere_center, 0.5, &material_right);
  hittable_list_add(&world, &sphere3);

  Camera camera;
  camera.aspect_ratio = 1.0;
  camera.samples_per_pixel = 10;
  camera.max_depth = 20;

  render(&world, &camera);

  hittable_list_clear(&world);
  material_clear(&material_ground);
  material_clear(&material_center);
  material_clear(&material_left);
  material_clear(&material_right);

  return 0;
}
