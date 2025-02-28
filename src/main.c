#include "../lib/MLX42/include/MLX42/MLX42.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "../include/render/color.h"
#include "../include/render/ray.h"
#include "../include/render/hit_record.h"
#include "../include/render/sphere.h"
#include "../include/render/utils.h"
#include "../include/render/hittable_list.h"


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

color_t     ray_color(const ray_t *r, const hittable_list_t *world) {
    hit_record_t    rec;
    if (hittable_list_hit_test(r, world, 0, INFINITY, &rec)) {
        return (vec3_scaled_return(vec3_sum(rec.normal, vec3(1,1,1)), 0.5));
    }
    vec3_t  unit_direction = vec3_normalize(r->dir);
    double  a = 0.5 * (unit_direction.y + 1.0);
    return(vec3_sum(vec3_scaled_return(vec3(1.0,1.0,1.0), 1.0-a), vec3_scaled_return(vec3(0.5,0.7,1.0), a)));
}

int main(void) {

    // MLX42

    mlx_t           *mlx;
    mlx_image_t     *image;

    // Image

    double      aspect_ratio = 16.0 / 9.0;
    int         image_width = 1000;

    // Calculate image height

    int image_height = (int)(image_width/aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    double              focal_length = 1.0;
    double              viewport_height = 2;
    double              viewport_width = viewport_height * ((double)image_width/image_height);
    point3_t            camera_center = point3(0, 0, 0);
    int                 samples_per_pixel = 10;
    double              pixel_sample_scale = 1.0 / samples_per_pixel; 

    // Vectors across horizontal and down the vertical viewport edges

    vec3_t viewport_u = vec3(viewport_width, 0, 0); 
    vec3_t viewport_v = vec3(0, -viewport_height, 0);

    // Pixel delta vectors

    vec3_t  pixel_delta_u = vec3_scaled_return(viewport_u, (1.0/image_width));
    vec3_t  pixel_delta_v = vec3_scaled_return(viewport_v, (1.0/image_height));

    // printf("image width:%f image height:%f\n", 1.0/image_width, 1.0/image_height);
    // printf("u_Delta:%f v_delta:%f\n", vec3_len(pixel_delta_u), vec3_len(pixel_delta_v));

    // Location of upper left pixel

    vec3_t      viewport_upper_left_pixel = vec3_sub_return(vec3_sub_return(vec3_sub_return(camera_center, vec3(0, 0, focal_length)), vec3_scaled_return(viewport_u, 0.5)), vec3_scaled_return(viewport_v, 0.5));
    point3_t    pixel00_loc = vec3_sum(viewport_upper_left_pixel, vec3_scaled_return(vec3_sum(pixel_delta_u, pixel_delta_v), 0.5));

    // World

    hittable_list_t     *world = hittable_list_innit(2);
    hittable_list_add(world, sphere_new(vec3(0,0,-1), 0.5));
    hittable_list_add(world, sphere_new(vec3(0,-100.5,-1), 100));

    // Render

    mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "miniRT", true);
    if (!mlx)
        return(1);
    image = mlx_new_image(mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!image || mlx_image_to_window(mlx, image, 0, 0) < 0)
        return(1);
    // mlx_close_hook(mlx, &esc_exit, NULL);
    for (int j = 0;j<image_height;++j){
        for (int i = 0;i<image_width;++i) {
            vec3_t color = vec3(0.0, 0.0, 0.0);
            for (int sample = 0;sample < samples_per_pixel;++sample) {
                ray_t r = get_ray(i, j, pixel00_loc, pixel_delta_u, pixel_delta_v, camera_center); 
                color = vec3_sum(color, ray_color(&r, world));
            }
            color = vec3_scaled_return(color, pixel_sample_scale);
            mlx_put_pixel(image, i, j, return_color(color));
        }
    }
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return(0);
}
