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

static inline bool vec3_near_zero(const vec3_t *a) {
    // Return true if the vector is close to zero in all dimensions
    int s = 1e-8;
    return ((abs(a->x) < s) && (abs(a->y) < s) && (abs(a->z) < s));
}

static inline vec3_t mirrored_reflect(const vec3_t *v, const vec3_t *n) {
    double scale = 2 * vec3_dot(*v, *n);
    return (vec3_sub_return(*v, vec3_scaled_return(*n, scale)));
}

// Point layer for vec3_t
typedef vec3_t point3_t;
#define point3(x, y ,z) vec3((x), (y), (z))


#endif
