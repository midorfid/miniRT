#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "../render_plane.h"
#include "../lens.h"

typedef struct ray_s {
    point3_t      orig;
    vec3_t        dir;
    double        time;
} ray_t;

static inline ray_t ray(point3_t orig, vec3_t dir, double time) {
    ray_t result = {.orig = orig, .dir = dir, .time = time};
    return (result);
}

static inline point3_t    ray_at(point3_t orig, vec3_t dir, double t) {
    return (vec3_sum(orig, vec3_scaled_return(dir, t)));
}

static inline vec3_t sample_square() {
    return vec3(random_double_nolimits() - 0.5, random_double_nolimits() - 0.5, 0);
}

static inline point3_t defocus_disk_sample(point3_t center, vec3_t defocus_disk_u, vec3_t defocus_disk_v) {
    point3_t p = random_in_unit_disk();

    return (vec3_sum(vec3_sum(center, (vec3_scaled_return(defocus_disk_u, p.x) )), vec3_scaled_return(defocus_disk_v, p.y)));
}

static inline vec3_t    sample_square_stratified(int s_i, int s_j, double rec1p_sqrt_spp) {
    // Returns the vector to a random point in the square sub-pixel specified by grid
    // indices s_i and s_j, for an idealized unit square pixel [-.5,-.5] to [+.5,+.5].
    double x = ((random_double_nolimits() + s_i) * rec1p_sqrt_spp) - 0.5;
    double y = ((random_double_nolimits() + s_j) * rec1p_sqrt_spp) - 0.5;

    return vec3(x, y, 0.0);
}

static inline ray_t get_ray(int height, int width, int s_i, int s_j, const render_plane_t *render_p, point3_t center, const defocus_blur_t *lens, double rec1p_sqrt_spp) {
    // Constructs a ray originating from the defocus disk and directed at ramdomly
    // sampled point around the pixel location i, j for stratified sample square s_i, s_j
    vec3_t offset = sample_square_stratified(s_i, s_j, rec1p_sqrt_spp);
    point3_t pixel_sample = vec3_sum(vec3_sum(render_p->pixel00_loc, vec3_scaled_return(render_p->pixel_delta_u, (width + offset.x))),
                                     vec3_scaled_return(render_p->pixel_delta_v, (height + offset.y)));    
    point3_t ray_origin = (lens->defocus_angle <= 0) ? center : defocus_disk_sample(center, lens->defocus_disk_u, lens->defocus_disk_v);    
    point3_t ray_dir = vec3_sub_return(pixel_sample, ray_origin);
    double time = random_double_nolimits();

    return ray(ray_origin, ray_dir, time);
}

#endif
