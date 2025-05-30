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
#include "../include/render/material.h"
#include "../include/render/material_metal.h"
#include "../include/render/material_lambertian.h"
#include "../include/render/material_dielectric.h"
#include "../include/camera/defocus_blur.h"
#include "../include/render/render_plane.h"
#include "../include/render/bvh.h"
#include <time.h>
#include "../include/render/box.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

color_t     ray_color(const ray_t *r, const hittable_list_t *world, int depth) {
    hit_record_t    rec;

    if (depth <= 0)
        return vec3(0.0, 0.0, 0.0);
    if (hittable_list_hit_test(r, world, 0.001, INFINITY, &rec)) {
        ray_t       scattered;
        color_t     attenuation;
        color_t     color_from_emmision = material_emmit(rec.mat, rec.u, rec.v, &rec.p);
        if (material_scatter(rec.mat, r, &rec, &attenuation, &scattered)) {
            color_t color_from_scatter = vec3_multi(ray_color(&scattered, world, depth-1), attenuation); 
            return vec3_sum(color_from_emmision, color_from_scatter);
        }
        return (color_from_emmision);
    }
    vec3_t  unit_direction = vec3_normalize(r->dir);
    double  a = 0.5 * (unit_direction.y + 1.0);
    return(vec3_sum(vec3_scaled_return(vec3(1.0,1.0,1.0), 1.0-a), vec3_scaled_return(vec3(0.5,0.7,1.0), a)));
}

int main(void) {

    // random generator seed set up
    uint64_t initial_seed = (uint64_t)time(NULL);
    rt_random_seed(initial_seed);

    // MLX42

    mlx_t           *mlx;
    mlx_image_t     *image;

    // Image

    double      aspect_ratio = 16.0 / 9.0;
    int         image_width = 400;

    // Calculate image height

    int image_height = (int)(image_width/aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    int                 samples_per_pixel = 10;
    double              pixel_sample_scale = 1.0 / samples_per_pixel; 
    int                 max_depth = 50;
    point3_t            lookfrom = point3(13,2,3);
    point3_t            lookat = point3(0,0,0);
    vec3_t              vup = vec3(0,1,0);
    // color_t             background = color_in(0.70, 0.80, 1.00); // TODO

    point3_t            camera_center = lookfrom;

    vec3_t              u,v,w; // camera frame basis vectors
    w = vec3_normalize(vec3_sub_return(lookfrom, lookat));
    u = vec3_cross(vup, w);
    v = vec3_cross(w, u);

    // Defocus Blur

    defocus_blur_t      lens;

    lens.defocus_angle = 0.6; // Variation angle of rays through each pixel
    double              focus_dist = 10.0; // Distance from camera lookfrom point to plane of perfect focus

    // Calculate camera defocus disk basis vectors
    double      defocus_radius = focus_dist * tan(DEG_TO_RAD(lens.defocus_angle / 2));
    lens.defocus_disk_u = vec3_scaled_return(u, defocus_radius); // defocus disk horizontal radius
    lens.defocus_disk_v = vec3_scaled_return(v, defocus_radius); // defocus disk vertical radius

    // Determine viewpoint dimensions

    double              vfov = 20;
    double              theta = DEG_TO_RAD(vfov);
    double              h = tan(theta/2);
    double              viewport_height = 2 * h * focus_dist;
    double              viewport_width = viewport_height * ((double)image_width/image_height);

    // Vectors across horizontal and down the vertical viewport edges

    vec3_t viewport_u = vec3_scaled_return(u, viewport_width);
    vec3_t viewport_v = vec3_scaled_return(vec3_negative(&v), viewport_height);

    // Pixel delta vectors

    render_plane_t      render_p;

    render_p.pixel_delta_u = vec3_scaled_return(viewport_u, (1.0/image_width));
    render_p.pixel_delta_v = vec3_scaled_return(viewport_v, (1.0/image_height));

    // printf("image width:%f image height:%f\n", 1.0/image_width, 1.0/image_height);
    // printf("u_Delta:%f v_delta:%f\n", vec3_len(pixel_delta_u), vec3_len(pixel_delta_v));

    // Location of upper left pixel

    vec3_t      viewport_upper_left_pixel = vec3_sub_return(vec3_sub_return(vec3_sub_return(camera_center, vec3_scaled_return(w, focus_dist)), vec3_scaled_return(viewport_u, 0.5)), vec3_scaled_return(viewport_v, 0.5));
    render_p.pixel00_loc = vec3_sum(viewport_upper_left_pixel, vec3_scaled_return(vec3_sum(render_p.pixel_delta_u, render_p.pixel_delta_v), 0.5));

    // World
    hittable_list_t     *world = hittable_list_innit(500);

    material_t  *material_ground = mt_lambertian_new(color_in(0.5, 0.5, 0.5));
    hittable_list_add(world, sphere_new(vec3(0.0,-1000.0, 0.0), 1000.0, material_ground));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            double choose_mat = random_double_nolimits();
            point3_t center = point3(a + 0.9 * random_double_nolimits(), 0.2, b + 0.9 * random_double_nolimits());

            if (vec3_len(vec3_sub_return(center, point3(4, 0.2, 0))) > 0.9) {
                material_t *material = NULL;
                hittable_t *object_to_add = NULL;
                if (choose_mat < 0.8) {
                    // diffuse
                    color_t albedo = vec3_multi(vec3_random(), vec3_random());
                    material = mt_lambertian_new(albedo);
                    point3_t center2 = (vec3_sum(center, vec3(0, random_double(0,.5), 0)));

                    object_to_add = mv_sphere_new(center, center2, 0, 1, 0.2, material);
                }
                else if (choose_mat < 0.95) {
                    // metal
                    color_t albedo2 = vec3_random_limits(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    material = mt_metal_new(albedo2, fuzz);

                    object_to_add = sphere_new(center, 0.2, material);
                }
                else {
                    // glass
                    material = mt_dielectric_new(1.5);

                    object_to_add = sphere_new(center, 0.2, material);
                }
                if (object_to_add != NULL)
                    hittable_list_add(world, object_to_add);
            }
        }
    }

    material_t *material1 = mt_dielectric_new(1.5);
    hittable_list_add(world, sphere_new(point3(0,1,0), 1.0, material1));

    material_t *material2 = mt_lambertian_new(color_in(0.4, 0.2, 0.1));
    hittable_list_add(world, sphere_new(point3(-4,1,0), 1.0, material2));

    material_t *material3 = mt_metal_new(color_in(0.7,0.6,0.5), 0.0);
    hittable_list_add(world, sphere_new(point3(4,1,0), 1.0, material3));

    hittable_list_t *result = hittable_list_innit(1);
    hittable_list_add(result, bvh_node_new(world, 0.0, 1.0));

    // Render

    mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "miniRT", true);
    if (!mlx) {
        fprintf(stderr, "Error initializing MLX42: %s\n", mlx_strerror(mlx_errno));
        return(1);
    }
    image = mlx_new_image(mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!image || mlx_image_to_window(mlx, image, 0, 0) < 0)
        return(1);
    // mlx_close_hook(mlx, &esc_exit, NULL);
    for (int j = 0;j<image_height;++j){
        for (int i = 0;i<image_width;++i) {
            vec3_t color = vec3(0.0, 0.0, 0.0);
            for (int sample = 0;sample < samples_per_pixel;++sample) {
                ray_t r = get_ray(i, j, &render_p, camera_center, &lens); 
                color = vec3_sum(color, ray_color(&r, world, max_depth));
            }
            color = vec3_scaled_return(color, pixel_sample_scale);
            mlx_put_pixel(image, i, j, return_color(color));
        }
    }
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return(0);
}
