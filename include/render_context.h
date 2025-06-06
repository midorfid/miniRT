#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "../lib/MLX42/include/MLX42/MLX42.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "../include/render/color.h"
#include "../include/render/ray.h"
#include "../include/render/hit_record.h"
#include "../include/render/sphere.h"
#include "../include/render/moving_sphere.h"
#include "../include/render/utils.h"
#include "../include/render/hittable_list.h"
#include "../include/render/bvh.h"
#include <time.h>
#include "../include/render/box.h"
#include "../include/scenes.h"
#include "../include/render_plane.h"
#include "../include/lens.h"


typedef struct image_s {
    double      aspect_ratio;
    int         image_width, image_height;
} image_t;

typedef struct viewpoint_s {
    double      vfov;
    double      theta;
    double      h;
    double      viewport_height, viewport_width;
    vec3_t      viewport_u, viewport_v;
} viewpoint_t;

typedef struct camera_s {
    int             samples_per_pixel;
    int             max_depth;
    double          pixel_sample_scale;
    point3_t        lookfrom, lookat;
    vec3_t          vup;
    vec3_t          u,v,w;  // camera frame basis vectors
    point3_t        camera_center;
    defocus_blur_t  lens;
} camera_t;

typedef struct render_context_s {
    mlx_t               *mlx;
    mlx_image_t         *mlx_image;

    //settings
    image_t             image;
    viewpoint_t         pov;
    camera_t            camera;
    render_plane_t      render_p;

    // scene objects
    hittable_list_t     *world;

} render_context_t;

typedef struct thread_data_s {
    int                 thread_id;
    int                 start_row, end_row;

    render_context_t    *context;
} thread_data_t;

render_context_t        *render_context_new(mlx_t *mlx, mlx_image_t *mlx_img);

static void        image_settings_init(image_t *image);

static void        lens_init(defocus_blur_t *lens, vec3_t u, vec3_t v);

static void        camera_settings_init(camera_t *camera);

static void        viewpoint_init(viewpoint_t *view_p, double focus_dist, double width_to_height_ratio, vec3_t u, vec3_t negative_v);

static void        render_plane_init(render_plane_t *render_p, vec3_t upper_left_pixel, image_t *image, vec3_t viewport_u, vec3_t viewport_v);

static void        render_context_init(render_context_t *render);

color_t            ray_color(const ray_t *r, const hittable_list_t *world, int depth);

#endif
