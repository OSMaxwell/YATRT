#ifndef RENDER_H
#define RENDER_H
#include "camera.h"
#include "hittable.h"

void single_core_render(const Hittable_list* world, Camera* camera);
void dual_core_render(const Hittable_list* world, Camera* camera);

#endif