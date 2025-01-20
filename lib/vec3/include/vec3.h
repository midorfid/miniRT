#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>

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

static inline void vec3_scale(vec3_t *a, double b) {
    a->x *= b.x;
    a->y *= b.y;
    a->z *= b.z;
}

static inline vec3_t vec3_scaled_return(vec3_t a, double b) {
    return (a.x * b, a.y * b, a.z * b);
}

static inline vec3_t vec3_negative(const vec3_t *a) {
    return (-a->x, -a->y, -a->z);
}

static inline vec3_t vec3_sum(vec3_t a, vec3_t b) {
    return (a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline double vec3_dot(vec3_t a, vec3_t b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}

static inline vec3_t vec3_cross(vec3_t a, vec3_t b) {
    return (a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

static inline vec3_t vec3_multi(vec3_t a, vec3_t b) {
    return (a.x * b.x, a.y * b.y, a.z * b.z);
}

static inline double vec3_len_squared(vec3_t a) {
    return (a.x * a.x + a.y * a.y + a.z * a.z);
}

static inline double vec3_len(vec3_t a) {
    return (sqrt(vec3_len_squared(a)));
}

#endif
