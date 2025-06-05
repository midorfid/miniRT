#ifndef SCENES_H
#define SCENES_H

#include "../include/render/hittable_list.h"
#include "../include/render/material.h"
#include "../include/render/material_metal.h"
#include "../include/render/material_lambertian.h"
#include "../include/render/material_dielectric.h"


typedef enum scene_id_s {
    SCENE_NONE = -1,
    SCENE_BOUNCING_SPHERES,
    SCENE_CORNELL_BOX_EMPTY,
    SCENE_CORNELL_BOX_STANDARD,
    SCENE_TOUCHING_SPHERES,
    SCENE_QUAD,
    SCENE_TWO_CHECKERED_SPHERES,
    SCENE_CORNELL_FOG,
    SCENE_SIMPLE_LIGHT,
    SCENE_PERLIN_SPHERES,
    SCENE_EARTH,
    SCENE_FINAL,
} scene_id_t;

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