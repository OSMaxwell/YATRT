#include "render.h"

#include "pico/multicore.h"
#include "pico/sync.h"
#include "pico/util/queue.h"

void single_core_render(const Hittable_list* world, Camera* camera) {
  clock_t now = clock();
  initialize(camera);

#ifdef __x86_64__
  // Init file stream
  FILE* output_file = fopen("image.ppm", "w");
  if (output_file == NULL) {
    perror("Error opening file");
    return;
  }
  // write image header
  fprintf(output_file, "P3\n%d %d\n255\n", IMAGE_WIDTH, IMAGE_HEIGHT);
#endif

  // Main render loop
  for (int i = 0; i < SCREEN_WIDTH; ++i) {
    for (int j = 0; j < SCREEN_HEIGHT; ++j) {
      Vec3 pixel_center = camera->pixel00_loc;
      Vec3 _tmp = vec3_scale_cpy(camera->pixel_delta_u, i);
      pixel_center = vec3_add(&pixel_center, &_tmp);
      _tmp = vec3_scale_cpy(camera->pixel_delta_v, j);
      pixel_center = vec3_add(&pixel_center, &_tmp);

      // Anti-aliasing
      Vec3 pixel_color = make_vec3_zero();
      for (int sample = 0; sample < camera->samples_per_pixel; ++sample) {
        Ray r = get_ray(camera, i, j);
        _tmp = colToVec(ray_color(&r, camera->max_depth, world));
        pixel_color = vec3_add(&pixel_color, &_tmp);
      }

      Color c = vecToCol(pixel_color);
#ifdef __arm__
      write_color(i, j, c, camera->samples_per_pixel);
#elif __x86_64__
      write_color(output_file, c, camera->samples_per_pixel);
#else
#endif
    }
  }
#ifdef __x86_64__
  fclose(output_file);
#endif
  now = clock() - now;
  printf("Done in  %d seconds %d milliseconds\n", (int)now / 1000,
         (int)now % 1000);
}

/*
  DUAL CORE
*/

typedef struct {
  Camera* pCamera;
  const Hittable_list* pHittable_list;
  int min_width;
  int max_width;
  Color color;
} render_options_t;

queue_t render_input_q;
semaphore_t spiSem;

void render(int width_min, int width_max, Camera* camera,
            const Hittable_list* world) {
  for (int i = width_min; i < width_max; ++i) {
    for (int j = 0; j < SCREEN_HEIGHT; ++j) {
      Vec3 pixel_center = camera->pixel00_loc;
      Vec3 _tmp = vec3_scale_cpy(camera->pixel_delta_u, i);
      pixel_center = vec3_add(&pixel_center, &_tmp);
      _tmp = vec3_scale_cpy(camera->pixel_delta_v, j);
      pixel_center = vec3_add(&pixel_center, &_tmp);

      // Anti-aliasing
      Vec3 pixel_color = make_vec3_zero();
      for (int sample = 0; sample < camera->samples_per_pixel; ++sample) {
        Ray r = get_ray(camera, i, j);
        _tmp = colToVec(ray_color(&r, camera->max_depth, world));
        pixel_color = vec3_add(&pixel_color, &_tmp);
      }

      Color col = vecToCol(pixel_color);
      sem_acquire_blocking(&spiSem);
      write_color(i, j, col, camera->samples_per_pixel);
      sem_release(&spiSem);
    }
  }
}

void core1_render_entry() {
  render_options_t options;
  printf("core 1: enter \n");
  clock_t now = clock();
  queue_remove_blocking(&render_input_q, &options);
  render(options.min_width, options.max_width, options.pCamera,
         options.pHittable_list);
  now = clock() - now;
  printf("Done in  %d seconds %d milliseconds\n", (int)now / 1000,
         (int)now % 1000);
  printf("core 1: done \n");
}

void dual_core_render(const Hittable_list* world, Camera* camera) {
  // implement core 1 and 2 entries with lock/mutex on SPI Bus
  sem_init(&spiSem, 1, 1);
  queue_init(&render_input_q, sizeof(render_options_t), 1);
  render_options_t input = {camera, world, 0, SCREEN_WIDTH / 2};
  initialize(camera);

  // Start render on 2nd core
  multicore_launch_core1(core1_render_entry);
  queue_add_blocking(&render_input_q, &input);

  // render the other half on 1st core
  clock_t now = clock();
  render(SCREEN_WIDTH / 2, SCREEN_WIDTH, camera, world);
  now = clock() - now;
  printf("Done in  %d seconds %d milliseconds\n", (int)now / 1000,
         (int)now % 1000);
  printf("core 0: done \n");
  while (1);
}