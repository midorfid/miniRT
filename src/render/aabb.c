#include "../../include/render/aabb.h"

bool                aabb_hit(const aabb_t *aabb, double tmin, double tmax, const ray_t *ray) {
    if (aabb == NULL) {
        printf(" aabb is NULL ");
        return (false);
    }
    for (vec3_axis_t axis = VEC3_AXIS_X; axis <= VEC3_AXIS_Z; ++axis) {
        double inverse_dir = 1.0 / ray->dir.components[axis];
        double t0 = (aabb->min.components[axis] - ray->orig.components[axis]) * inverse_dir ,
               t1 = (aabb->max.components[axis] - ray->orig.components[axis]) * inverse_dir;
        if (t0 > t1) {
            double temp = t0;
            t0 = t1;
            t1 = temp;
        }
        tmin = tmin > t0 ? t0 : tmin;
        tmax = tmax < t1 ? t1 : tmax;
        if (tmax <= tmin)
            return false;
    }
    return true;
}

aabb_t      aabb_surrounding_bbox(aabb_t a, aabb_t b) {
    aabb_t result = {.min = 
                        {
                            fmin(a.min.x, b.min.x),
                            fmin(a.min.y, b.min.y),
                            fmin(a.min.z, b.min.z),
                        },
                    .max = {
                            fmax(a.max.x, b.max.x),
                            fmax(a.max.y, b.max.y),
                            fmax(a.max.z, b.max.z), 
                    }};
    return result;
}

aabb_t      aabb_two_points(point3_t a, point3_t b) {
    aabb_t  result = 
    {
        .min = a,
        .max = b,
    };
    return result;
}
