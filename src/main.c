// #include <Adafruit_GFX.h>
// #include <Adafruit_ST7735.h>

#include <stdio.h>

// YATRT
#include "camera.h"
#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "render.h"
#include "sphere.h"

// PICO
#include "ST7735_TFT.h"
#include "hardware/spi.h"
#include "hw.h"

const float pi = 3.141592653589;
const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 160;

void init() {
#ifdef __arm__
  stdio_init_all();
  spi_init(SPI_PORT, 1000000);  // SPI with 1Mhz
  gpio_set_function(SPI_RX, GPIO_FUNC_SPI);
  gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);
  gpio_set_function(SPI_TX, GPIO_FUNC_SPI);
  tft_spi_init();
#ifdef TFT_ENABLE_BLACK
  TFT_BlackTab_Initialize();
#elif defined(TFT_ENABLE_GREEN)
  TFT_GreenTab_Initialize();
#elif defined(TFT_ENABLE_RED)
  TFT_RedTab_Initialize();
#elif defined(TFT_ENABLE_GENERIC)
  TFT_ST7735B_Initialize();
#endif
  setTextWrap(true);
  sleep_ms(300);
  fillScreen(ST7735_BLACK);

  setRotation(0);
#endif
}

int main(void) {
#ifdef TESTSUITE
  if (testcases() != 0) {
    printf("FAIL!\n");
    while (1);
  }
  printf("PASS\n");
#endif
  init();
  printf("Init done.\n");

  // World
  // Material
  Material material_ground = make_lambertian((Color){0.8, 0.8, 0.0});
  Material material_center = make_lambertian((Color){0.7, 0.3, 0.3});
  Material material_left = make_metal((Color){0.5, 0.5, 0.75}, 0.3);
  Material material_right = make_metal((Color){0.8, 0.6, 0.2}, 1.0);

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

  // Settings
  Camera camera;
  camera.aspect_ratio = 1.0;
  camera.samples_per_pixel = 20;
  camera.max_depth = 10;

  printf("Starting render Loop\n");
  #ifdef SINGLE_CORE
  single_core_render(&world, &camera);
  #endif
  #ifdef DUAL_CORE
  dual_core_render(&world, &camera);
  #endif

  hittable_list_clear(&world);
  material_clear(&material_ground);
  material_clear(&material_center);
  material_clear(&material_left);
  material_clear(&material_right);

  return 0;
}
