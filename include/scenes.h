#ifndef SCENES_H
#define SCENES_H

#include "../include/render/hittable_list.h"

hittable_list_t             *earth();
hittable_list_t             *quads();
hittable_list_t             *cornell_smoke();
hittable_list_t             *final_scene(int image_width, int samples_per_pixel, int max_depth);
hittable_list_t             *simple_light();
hittable_list_t             *perlin_spheres();
hittable_list_t             *cornell_box_empty();
hittable_list_t             *cornell_box_standard();
hittable_list_t             *checkered_spheres();
hittable_list_t             *touching_spheres();
hittable_list_t             *bouncing_spheres();

#endif