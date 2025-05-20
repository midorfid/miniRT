#include "../../include/render/sphere.h"
#include "../../include/render/hit_record.h"
#include <assert.h>
#include <stdlib.h>
#include "../../include/render/ray.h"

// p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
void    get_sphere_uv(const point3_t *p, double *u, double *v) {
    double theta = -acos(p->y);
    double phi = atan2(-p->z, p->x) + PI;

    *u = phi / PI*2;
    *v = theta / PI;
}

/*  (C - P)(C - P) = r^2 , where C = center of the sphere and P = any point that satisfies this equation is on the sphere
    (C - (Q + td))(C - (Q + td)) = r^2 , where Q = origin, d = direction, t = our unknown, basically scalar for which this equation is true
    (-td + (C - Q))(-td + (C - Q)) = r^2 , then we calculate dot product of a vector with it self
    t^2d*d - 2td(C - Q) + (C - Q)(C - Q) - r^2 = 0 then we need to find discriminant
*/  
bool sphere_hit_test_generic(point3_t center, double radius, material_t *material, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
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

    if (rec != NULL) {
        rec->mat = material;
        rec->t = root;
        rec->p = ray_at(ray->orig, ray->dir, rec->t);
        vec3_t outward_normal = vec3_scaled_return(vec3_sub_return(rec->p, center), 1.0/radius);
        set_front_face(ray, &outward_normal, rec);
        get_sphere_uv(&outward_normal, &rec->u, &rec->v);
    }
    return (true);
}

static  sphere_t  sphere_init(point3_t center, double radius, material_t *material) {
    sphere_t    result = {.center = center, .radius = radius, .material = material};

    hittable_innit(&result.base, HITTABLE_TYPE_SHPERE, sphere_hit, bb_base, delete_base);
    return (result);
}

hittable_t    *sphere_new(point3_t center, double radius, material_t *material) {
    sphere_t        *new_sphere = calloc(1, sizeof(sphere_t));
    if (new_sphere == NULL) {
        printf("Memory allocation failed");
        return (NULL);
    }
    *new_sphere = sphere_init(center, radius, material);
    return (hittable_t *)new_sphere;
}

static  bool     sphere_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    if (hittable->type != HITTABLE_TYPE_SHPERE) {
        printf("WRONG TYPE (SPHERE)\n");
        return (false);
    }
    sphere_t *sphere = (sphere_t *)hittable;
    return (sphere_hit_test_generic(sphere->center, sphere->radius, sphere->material, ray, tmin, tmax, rec));
}
