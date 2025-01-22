#include "../lib/MLX42/include/MLX42/MLX42.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/render/color.h"
#include "../include/render/ray.h"


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

/*  (C - P)(C - P) = r^2 , where C = center of the sphere and P = any point that satisfies this equation is on the sphere
    (C - (Q + td))(C - (Q + td)) = r^2 , where Q = origin, d = direction, t = our unknown, basically scalar for which this equation is true
    (-td + (C - Q))(-td + (C - Q)) = r^2 , then we calculate dot product of a vector with it self
    t^2d*d - 2td(C - Q) + (C - Q)(C - Q) - r^2 = 0 then we need to find discriminant
*/  
double hit_sphere(const point3_t center, double radius, const ray_t *ray) {
    vec3_t oc = ray->orig;
    vec3_sub(&oc, center); 
    double a = vec3_dot(ray->dir,ray->dir);//
    double b = 2.0 * vec3_dot(ray->dir, oc);
    double c = vec3_dot(oc, oc) - radius*radius;

    double discriminant = b*b - 4*a*c;
    if (discriminant < 0)
        return (-1.0);
    else
        return ((-b - sqrt(discriminant)) / 2.0 * a);
}

color_t     ray_color(const ray_t *r) {
    double t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        vec3_t n = vec3_normalize(vec3_sub_return(ray_at(r->orig,r->dir,t), vec3(0,0,-1)));
        return (vec3_scaled_return(vec3(n.x + 1, n.y + 1, n.z + 1), 0.5));
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
    int         image_width = 400;

    // Calculate image height

    int image_height = (int)(image_width/aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    double      focal_length = 1.0;
    double      viewport_height = 2;
    double      viewport_width = viewport_height * ((double)image_width/image_height);
    point3_t    camera_center = point3(0, 0, 0);

    // Vectors across horizontal and down the vertical viewport edges

    vec3_t viewport_u = vec3(viewport_width, 0, 0); 
    vec3_t viewport_v = vec3(0, -viewport_height, 0);

    // Pixel delta vectors

    vec3_t  pixel_delta_u = vec3_scaled_return(viewport_u, (1.0/image_width));
    vec3_t  pixel_delta_v = vec3_scaled_return(viewport_v, (1.0/image_height));

    printf("image width:%f image height:%f\n", 1.0/image_width, 1.0/image_height);
    printf("u_Delta:%f v_delta:%f\n", vec3_len(pixel_delta_u), vec3_len(pixel_delta_v));

    // Location of upper left pixel

    vec3_t      viewport_upper_left_pixel = vec3_sub_return(vec3_sub_return(vec3_sub_return(camera_center, vec3(0, 0, focal_length)), vec3_scaled_return(viewport_u, 0.5)), vec3_scaled_return(viewport_v, 0.5));
    point3_t    pixel00_loc = vec3_sum(viewport_upper_left_pixel, vec3_scaled_return(vec3_sum(pixel_delta_u, pixel_delta_v), 0.5));

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
            vec3_t      pixel_center = vec3_sum(vec3_sum(pixel00_loc, vec3_scaled_return(pixel_delta_u, i)), vec3_scaled_return(pixel_delta_v, j));
            point3_t    ray_direction = vec3_sub_return(pixel_center, camera_center);
            ray_t       r = ray(camera_center, ray_direction);

            int color = return_color(ray_color(&r));
            mlx_put_pixel(image, i, j, color);
        }
    }
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return(0);
}
