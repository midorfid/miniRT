#include "../../include/render/sphere.h"
#include "../../include/render/hit_record.h"
#include <stdlib.h>
#include "../../include/render/ray.h"

/*  (C - P)(C - P) = r^2 , where C = center of the sphere and P = any point that satisfies this equation is on the sphere
    (C - (Q + td))(C - (Q + td)) = r^2 , where Q = origin, d = direction, t = our unknown, basically scalar for which this equation is true
    (-td + (C - Q))(-td + (C - Q)) = r^2 , then we calculate dot product of a vector with it self
    t^2d*d - 2td(C - Q) + (C - Q)(C - Q) - r^2 = 0 then we need to find discriminant
*/  
static inline bool hit_sphere(sphere_t sphere, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    vec3_t oc = sphere.center;
    vec3_sub(&oc, ray->orig); 
    double a = vec3_len_squared(ray->dir);
    double h = vec3_dot(ray->dir, oc);
    double c = vec3_len_squared(oc) - sphere.radius*sphere.radius;

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

    rec.t = root;
    rec->p = ray_at(ray->orig, ray->dir, t);
    vec3_t outward_normal = vec3_scaled_return(vec3_sub_return(rec->p, sphere.center), 1.0/sphere.radius);
    rec->normal = set_front_face(ray, &outward_normal, rec);
    return (true);
}

static inline sphere_t  sphere_init(point3_t center, double radius) {
    sphere_t    result = {.center = center, .radius = radius};

    hittable_innit(&result.base, HITTABLE_TYPE_SHPERE, hit_sphere);
    return (result)
}

static inline hittable_t    *sphere_new(point3_t center, double radius) {
    sphere_t        *new_sphere = calloc(1, sizeof(sphere_t));
    if (new_sphere == NULL) {
        printf("Memory allocation failed");
        return (NULL);
    }
    *sphere = sphere_init(center, radius);
    return (hittable_t *)new_sphere;
}
