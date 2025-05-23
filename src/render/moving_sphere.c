#include "../include/render/moving_sphere.h"

static mv_sphere_t      mv_sphere_init(point3_t center_start, point3_t center_end, double time_start, double time_end, double radius, material_t *material) {
    mv_sphere_t     result = {
        .center_start = center_start,
        .center_end = center_end,
        .time_start = time_start,
        .time_end = time_end,
        .radius = radius,
        .material = material};

    hittable_innit(&result.base, HITTABLE_TYPE_MOVING_SHPERE, mv_sphere_hit, mv_sphere_bb, mv_sphere_delete);
    return (result);
}


hittable_t                  *mv_sphere_new(point3_t center_start, point3_t center_end, double time_start, double time_end, double radius, material_t *material) {
    mv_sphere_t *new_sphere = calloc(1, sizeof(mv_sphere_t));
    if (new_sphere == NULL) {
        printf(" allocation of mem for mv sphere failed ");
        return (NULL);
    }
    *new_sphere = mv_sphere_init(center_start, center_end, time_start, time_end, radius, material);
    return (hittable_t *)new_sphere;
}

static bool          mv_sphere_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    if (hittable->type != HITTABLE_TYPE_MOVING_SHPERE) {
        printf(" wrong type for mv sphere hit ");
        return (false);
    }
    mv_sphere_t *sphere = (mv_sphere_t *)hittable;
    point3_t center = get_center_at_time(sphere, ray->time);
    return sphere_hit_test_generic(center, sphere->radius, sphere->material, ray, tmin, tmax, rec);
}

static point3_t get_center_at_time (const mv_sphere_t *sphere, double time) {
    if (sphere == NULL) {
        printf(" parameter: mv sphere is NULL ");
        return (vec3(0.0 , 0.0 , 0.0));
    }
    double scale = (time - sphere->time_start) / (sphere->time_end - sphere->time_start);
    return (vec3_sum(sphere->center_start, vec3_scaled_return(vec3_sub_return(sphere->center_end, sphere->center_start), scale)));
}

bool                        mv_sphere_bb(const hittable_t *hittable, double time0, double time1, aabb_t *out_bb) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_MOVING_SHPERE || out_bb == NULL) {
        printf("my_sphere_bb() failed");
        return false;
    }
    mv_sphere_t *mv_sphere = (mv_sphere_t *)hittable;
    
    vec3_t rvec = vec3(mv_sphere->radius, mv_sphere->radius, mv_sphere->radius);
    aabb_t a = 
    {
        .min = vec3_sub_return(get_center_at_time(mv_sphere, time0), rvec),
        .max = vec3_sum(get_center_at_time(mv_sphere, time0), rvec),
    };
    aabb_t b = 
    {
        .min = vec3_sub_return(get_center_at_time(mv_sphere, time1), rvec),
        .max = vec3_sum(get_center_at_time(mv_sphere, time1), rvec),
    };
    *out_bb = aabb_surrounding_bbox(a, b);

    return true;
}

void                        mv_sphere_delete(hittable_t *hittable) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_MOVING_SHPERE) {
        printf("mv_sphere_delete() failed");
        return ;
    }
    mv_sphere_t *delete_sphere = (mv_sphere_t *)hittable;
    
    material_delete(delete_sphere->material);
    free(delete_sphere);
}
