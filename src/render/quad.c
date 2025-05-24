#include "../../include/render/quad.h"

static my_quad_t       quad_init(point3_t Q, vec3_t u, vec3_t v, material_t *material) {
   my_quad_t result = 
   {
        .Q = Q,
        .u = u,
        .v = v,
        .material = material,
   };
   result.normal = vec3_normalize(vec3_cross(result.u, result.v));
   result.D = vec3_dot(result.normal, result.Q);

   hittable_innit(&result.base, HITTABLE_TYPE_QUAD, quad_hit, quad_bb, quad_delete);

   return result;
}

static bool         quad_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_QUAD) {
            printf("quad_hit() failed");
            return false;
    }
    my_quad_t *quad = (my_quad_t *)hittable;
    
    double  denom = vec3_dot(quad->normal, ray->dir);
    // if ray is 0 or close to zero, then it never intersects. fabs() in case if ray intersects from opposite side of normal
    if (fabs(denom) < 1e-8)
        return false;
    double t = (quad->D - vec3_dot(quad->normal, ray->orig)) / denom;
    // intersection is not within the valid segment of the ray
    if (t < tmin || t > tmax)
        return false;
    point3_t intersection = ray_at(ray->orig, ray->dir, t);

    rec->t = t;
    rec->p = intersection;
    rec->mat = quad->material;
    set_front_face(ray, quad->normal, rec);

    return true;
}

hittable_t          *quad_new(point3_t Q, vec3_t u, vec3_t v, material_t *material) {
    my_quad_t *new_quad = calloc(1, sizeof(my_quad_t));
    if (new_quad == NULL) {
        printf("quad_new() memory alloc failed");
        return NULL;
    }
    *new_quad = quad_init(Q, u, v, material);
 
    return (hittable_t *)new_quad;
}

void                quad_delete(hittable_t *hittable) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_QUAD) {
        printf("quad_delete() failed");
        return ;
    }
    my_quad_t *quad = (my_quad_t *)hittable;

    material_delete(quad->material);
    free(quad);
}

bool                quad_bb(const hittable_t *hittable, double time0, double time1, aabb_t *out_box) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_QUAD || out_box == NULL) {
        printf("quad_bb() failed");
        return false;
    }
    my_quad_t *quad = (my_quad_t *)hittable;

    aabb_t diagonal1 = 
    {
        .min = quad->Q,
        .max = vec3_sum(quad->Q, vec3_sum(quad->u, quad->v)),
    };
    aabb_t diagonal2 = 
    {
        .min = vec3_sum(quad->Q, quad->u),
        .max = vec3_sum(quad->Q, quad->v),
    };
    *out_box = aabb_surrounding_bbox(diagonal1, diagonal2);

    return true;
}
