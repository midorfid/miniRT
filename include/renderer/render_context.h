#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include "MLX42.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "color.h"
#include "core/ray.h"
#include "core/hit_record.h"
#include "geometry/objects/sphere.h"
#include "geometry/objects/moving_sphere.h"
#include "math/utils.h"
#include "geometry/objects/hittable_list.h"
#include "geometry/bvh.h"
#include <time.h>
#include "geometry/objects/box.h"
#include "system/scenes.h"
#include "renderer/render_plane.h"
#include "core/lens.h"
#include "renderer/threads/thread.h"
#include "renderer/threads/thread_pool.h"
#include "shading/pdfs/pdf_cosine.h"
#include "shading/pdfs/pdf_hittable.h"
#include "shading/pdfs/pdf_mixtures.h"


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
    int             sqrt_spp; // sample per pixel
    double          rec1p_sqrt_spp;
    point3_t        lookfrom, lookat;
    vec3_t          vup;
    vec3_t          u,v,w;  // camera frame basis vectors
    point3_t        camera_center;
    defocus_blur_t  lens;
} camera_t;

// typedef color_t (*render_fn_t)()

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
    hittable_list_t     *lights;

    // threads
    pthread_mutex_t          *process_mutex;
    int                 *processed_chunks;
    int                 total_chunks;
    uint64_t            random_seed;
    // render_fn_t         render_function;

    // chunk
    vec3_t              *chunk_buffer;
    int                 width, height;
    int                 x_start, y_start;
} render_context_t;


render_context_t        *render_context_new(render_context_t *render_contxt, mlx_t *mlx, mlx_image_t *mlx_img);

static void        image_settings_init(image_t *image);

static void        lens_init(defocus_blur_t *lens, vec3_t u, vec3_t v);

static void        camera_settings_init(camera_t *camera);

static void        viewpoint_init(viewpoint_t *view_p, double focus_dist, double width_to_height_ratio, vec3_t u, vec3_t negative_v);

static void        render_plane_init(render_plane_t *render_p, vec3_t upper_left_pixel, image_t *image, vec3_t viewport_u, vec3_t viewport_v);

static void        render_context_init(render_context_t *render);

color_t            ray_color(const ray_t *r, const hittable_list_t *world, const hittable_list_t *lights, int depth);

#endif
