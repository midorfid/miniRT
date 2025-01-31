#include "../../include/render/sphere.h"
#include "../../include/render/hit_record.h"
#include <assert.h>
#include <stdlib.h>
#include "../../include/render/ray.h"

/*  (C - P)(C - P) = r^2 , where C = center of the sphere and P = any point that satisfies this equation is on the sphere
    (C - (Q + td))(C - (Q + td)) = r^2 , where Q = origin, d = direction, t = our unknown, basically scalar for which this equation is true
    (-td + (C - Q))(-td + (C - Q)) = r^2 , then we calculate dot product of a vector with it self
    t^2d*d - 2td(C - Q) + (C - Q)(C - Q) - r^2 = 0 then we need to find discriminant
*/  
static inline bool sphere_hit_test_generic(point3_t center, double radius, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    vec3_t oc = center;
    vec3_sub(&oc, ray->orig); 
    double a = vec3_len_squared(ray->dir);
    double h = vec3_dot(ray->dir, oc);
    double c = vec3_len_squared(oc) - radius*radius;

    double discriminant = h*h - a*c;
    if (discriminant < 0)
        return (false);
    
    // Find the nearest root

    double root = (h - sqrt(discriminant)) / a;
    if (root <= tmin || root >= tmax) {
        root = (h + sqrt(discriminant)) / a;
        if (root <= tmin || root >= tmax)
            return (false);
    }

    rec->t = root;
    rec->p = ray_at(ray->orig, ray->dir, rec->t);
    vec3_t outward_normal = vec3_scaled_return(vec3_sub_return(rec->p, center), 1.0/radius);
    set_front_face(ray, &outward_normal, rec);
    return (true);
}

static inline sphere_t  sphere_init(point3_t center, double radius) {
    sphere_t    result = {.center = center, .radius = radius};

    hittable_innit(&result.base, HITTABLE_TYPE_SHPERE, sphere_hit);
    return (result);
}

hittable_t    *sphere_new(point3_t center, double radius) {
    sphere_t        *new_sphere = calloc(1, sizeof(sphere_t));
    if (new_sphere == NULL) {
        printf("Memory allocation failed");
        return (NULL);
    }
    *new_sphere = sphere_init(center, radius);
    return (hittable_t *)new_sphere;
}

static inline bool     sphere_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    if (hittable->type != HITTABLE_TYPE_SHPERE) {
        printf("WRONG TYPE (SPHERE)\n");
        return (false);
    }
    sphere_t *sphere = (sphere_t *)hittable;
    return (sphere_hit_test_generic(sphere->center, sphere->radius, ray, tmin, tmax, rec));
}
