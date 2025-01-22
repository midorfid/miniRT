#include "../../include/render/sphere.h"

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
        return (-1.0);
    else
        return ((h - sqrt(discriminant)) / a);
}