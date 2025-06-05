#include  "../../include/render/constant_medium.h"

bool        const_medium_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_CONST_MEDIUM) {
        printf("const_medium_hit() failed");
        return false;
    }
    const_medium_t *medium = (const_medium_t *)hittable;
    hit_record_t    fog_start, fog_end;
    for (double tsearch_min = tmin; tsearch_min < tmax; tsearch_min = fog_end.t + 0.0001) {
        if (!hittable_t_hit(medium->boundary, ray, -INFINITY, INFINITY, &fog_start))
            return false;
        if (!hittable_t_hit(medium->boundary, ray, fog_start.t + 0.0001, INFINITY, &fog_end))
            return false;
        if (fog_start.t < tsearch_min)
            fog_start.t = tsearch_min;
        if (fog_end.t > tmax)
            fog_end.t = tmax;
        if (fog_start.t >= fog_end.t)
            continue;
        if (fog_start.t < 0)
            fog_start.t = 0;

        double ray_len = vec3_len(ray->dir);
        double dist_inside_boundary = (fog_end.t - fog_start.t) * ray_len;
        double hit_dist = medium->inv_neg_density * log(random_double_nolimits());
        
        if (hit_dist <= dist_inside_boundary) {
            rec->t = fog_start.t + hit_dist / ray_len;
            rec->p = ray_at(ray->orig, ray->dir, rec->t);
            
            rec->normal = vec3(1, 0, 0);
            rec->front_face = true;
            rec->mat = medium->phase_function;

            return true;
        }
    }
    return false;
}

void        const_medium_delete(hittable_t *hittable) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_CONST_MEDIUM) {
        printf("const_medium_delete() failed");
        return ;
    }
    const_medium_t *todelete = (const_medium_t *)hittable;
    hittable_delete(todelete->boundary);
    free(todelete);
}

bool        const_medium_bb(const hittable_t *hittable, double time0, double time1, aabb_t *out_bbox) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_CONST_MEDIUM) {
        printf("const_medium_bb() failed");
        return false;
    }
    const_medium_t *medium = (const_medium_t *)hittable;

    return hittable_t_bb(medium->boundary, time0, time1, out_bbox);
}

hittable_t  *const_medium_new_with_tex(hittable_t *boundary, double density, texture_t *texture) {
    const_medium_t  *medium = calloc(1, sizeof(const_medium_t));
    if (medium == NULL) {
        printf("const_medium_new() failed");
        return NULL;
    }
    medium->boundary = boundary;
    medium->inv_neg_density = -1.0 / density;
    // medium->phase_function = TODO
    hittable_innit(&medium->base, HITTABLE_TYPE_CONST_MEDIUM, const_medium_hit, const_medium_bb, const_medium_delete);

    return (hittable_t *)medium;
}

hittable_t  *const_medium_new_with_colour(hittable_t *boundary, double density, double albedo) {
    // return const_medium_new_with_tex(boundary, density, texture) TODO
}
