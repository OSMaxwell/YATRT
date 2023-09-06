#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "common.h"
#include "hittable.h"

typedef struct hittable_list {
  Hittable *objects;
  size_t count;
} Hittable_list;

void hittable_list_init(Hittable_list *list) {
  list->objects = NULL;
  list->count = 0;
}

void hittable_list_clear(Hittable_list *list) {
  // Free memory and decrease reference counts
  Hittable *objects = list->objects;
  for (size_t i = 0; i < list->count; i++) {
    // cleanup if needed
    free(objects[i].object);
  }

  free(list->objects);
  list->objects = NULL;
  list->count = 0;
}

void hittable_list_add(Hittable_list *list, Hittable *object) {
  list->count++;
  if (list->objects == NULL)
    list->objects = (Hittable *)malloc(list->count * sizeof(Hittable));
  else
    list->objects =
        (Hittable *)realloc(list->objects, list->count * sizeof(Hittable));
  list->objects[list->count - 1] = *object;
}

// Function to check if a ray hits a hittable object
bool hittable_hit(const Hittable *hittable, const Ray *ray, Interval ray_t,
                  hit_record *record) {
  return hittable->hit_function(hittable->object, ray, ray_t, record);
}

bool hittable_list_hit(const Hittable_list *list, const Ray *r, Interval ray_t,
                       hit_record *rec) {
  hit_record temp_rec;
  bool hit_anything = false;
  float closest_so_far = ray_t.max;

  for (size_t i = 0; i < list->count; i++) {
    Hittable *obj = list->objects + i;
    if (hittable_hit(obj, r, make_interval(ray_t.min, closest_so_far),
                     &temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      *rec = temp_rec;
    }
  }

  return hit_anything;
}

#endif