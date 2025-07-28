#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "utils.h"
#include <stdbool.h>

typedef enum vec3_axis_e
{
    VEC3_AXIS_X = 0,
    VEC3_AXIS_Y = 1,
    VEC3_AXIS_Z = 2
} vec3_axis_t;

union vec3_u
{
    double components[3];
    struct {
        double  x;
        double  y;
        double  z;
    };  
};

typedef union vec3_u vec3_t;

static inline vec3_t vec3(double x, double y, double z) {
    vec3_t result = {.x = x, .y = y, .z = z};
    return (result);
}

static inline void vec3_add(vec3_t *a, vec3_t b) {
    a->x += b.x;
    a->y += b.y;
    a->z += b.z;
}

static inline void vec3_sub(vec3_t *a, vec3_t b) {
    a->x -= b.x;
    a->y -= b.y;
    a->z -= b.z;
}

static inline vec3_t vec3_sub_return(vec3_t a, vec3_t b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline void vec3_scale(vec3_t *a, double b) {
    a->x *= b;
    a->y *= b;
    a->z *= b;
}

static inline vec3_t vec3_scaled_return(vec3_t a, double b) {
    return vec3(a.x * b, a.y * b, a.z * b);
}

static inline vec3_t vec3_negative(const vec3_t *a) {
    return vec3(-a->x, -a->y, -a->z);
}

static inline vec3_t vec3_sum(vec3_t a, vec3_t b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline double vec3_dot(vec3_t a, vec3_t b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

static inline vec3_t vec3_cross(vec3_t a, vec3_t b) {
    return vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

static inline vec3_t vec3_multi(vec3_t a, vec3_t b) {
    return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

static inline double vec3_len_squared(vec3_t a) {
    return (a.x * a.x + a.y * a.y + a.z * a.z);
}

static inline double vec3_len(vec3_t a) {
    return (sqrt(vec3_len_squared(a)));
}

static inline vec3_t vec3_normalize(vec3_t a) {
    return (vec3_scaled_return(a, 1.0 / vec3_len(a)));
}

static inline vec3_t vec3_random_limits(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

static inline vec3_t vec3_random(void) {
    return vec3(random_double_nolimits(), random_double_nolimits(), random_double_nolimits());
}

static inline vec3_t vec3_random_unit_vec() {
    while (1) {
        vec3_t p = vec3_random();
        double lensq = vec3_len_squared(p);
        if (1e-160 < lensq && lensq <= 1) {
            return vec3_scaled_return(p, 1.0 / sqrt(lensq));
        }
    }
}

static inline vec3_t vec3_random_on_hemisphere(vec3_t *normal) {
    vec3_t on_unit_shpere = vec3_random_unit_vec();
    if (vec3_dot(on_unit_shpere, *normal) > 0.0)
        return (on_unit_shpere);
    else
        return (vec3_scaled_return(on_unit_shpere, -1.0));
}

static inline vec3_t random_in_unit_disk() {
    while(true) {
        vec3_t p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (vec3_len_squared(p) < 1)
            return (p);
    }
}

static inline bool vec3_near_zero(const vec3_t *a) {
    // Return true if the vector is close to zero in all dimensions
    int s = 1e-8;
    return ((abs(a->x) < s) && (abs(a->y) < s) && (abs(a->z) < s));
}

static inline vec3_t reflect(const vec3_t *v, const vec3_t *n) {
    double scale = 2 * vec3_dot(*v, *n);
    return (vec3_sub_return(*v, vec3_scaled_return(*n, scale)));
}
/* Snell's Law
    eta * sin(theta) = eta prime * sin(theta prime)*/
static inline vec3_t refract(const vec3_t *uv, const vec3_t *n, double eta_over_etap) {
    double cos_theta = fmin(vec3_dot(vec3_negative(uv), *n), 1);
    vec3_t r_out_perp = vec3_scaled_return(vec3_sum(*uv, vec3_scaled_return(*n, cos_theta)), eta_over_etap);
    vec3_t r_out_parallel = vec3_scaled_return(*n, -1 * sqrt(fabs(1 - vec3_len_squared(r_out_perp))));
    return(vec3_sum(r_out_perp, r_out_parallel));
}

static inline vec3_t random_cosine_direction() {
    double r1 = random_double(0.0, 1.0);
    double r2 = random_double(0.0, 1.0);

    double phi = 2 * PI * r1;
    double x = cos(phi) * sqrt(r2);
    double y = sin(phi) * sqrt(r2);
    double z = sqrt(1 - r2);

    return vec3(x,y,z);
}

// Point layer for vec3_t
typedef vec3_t point3_t;
#define point3(x, y ,z) vec3((x), (y), (z))


#endif
