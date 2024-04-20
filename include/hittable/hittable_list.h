#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "common.h"
#include "hittable.h"

typedef struct hittable_list {
  Hittable *objects;
  size_t count;
} Hittable_list;

void hittable_list_init(Hittable_list *list);

void hittable_list_clear(Hittable_list *list);

void hittable_list_add(Hittable_list *list, Hittable *object);

// Function to check if a ray hits a hittable object
bool hittable_hit(const Hittable *hittable, const Ray *ray, Interval ray_t,
                  hit_record *record);

bool hittable_list_hit(const Hittable_list *list, const Ray *r, Interval ray_t,
                       hit_record *rec);

#endif