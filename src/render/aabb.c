#include "../../include/render/aabb.h"

bool                aabb_hit(const aabb_t *aabb, double tmin, double tmax, const ray_t *ray) {
    if (aabb == NULL) {
        printf(" aabb is NULL ")
        return (false);
    }
    for (axis_t axis = AXIS_X; axis <= AXIS_Z; ++axis) {
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

aabb_t      aabb_surrounding_bbox(const aabb_t *a, const aabb_t *b) {
    aabb_t result = {.min = 
                        {
                            fmin(a->min.x, b->min.x),
                            fmin(a->min.y, b->min.y),
                            fmin(a->min.z, b->min.z),
                        },
                    .max = {
                            fmax(a->max.x, b->max.x),
                            fmax(a->max.y, b->max.y),
                            fmax(a->max.z, b->max.z), 
                    }};
    return result;
}