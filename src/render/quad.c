#include "../../include/render/quad.h"

static my_quad_t       quad_init(point3_t Q, vec3_t u, vec3_t v, material_t *material) {
   my_quad_t result = 
   {
        .Q = Q,
        .u = u,
        .v = v,
        .material = material,
   };
   vec3_t n = vec3_cross(result.u, result.v);
   result.normal = vec3_normalize(n);
   result.D = vec3_dot(result.normal, result.Q);
   result.w = vec3_scaled_return(n, 1 / vec3_dot(n,n));
   
   result.area = vec3_len(n);

   result.u_len_squared = vec3_len_squared(u);
   result.v_len_squared = vec3_len_squared(v);

   hittable_innit(&result.base, HITTABLE_TYPE_QUAD, quad_hit, quad_bb, quad_delete, quad_pdf_value, quad_random);

   return result;
}

static bool         is_interior(double a, double b, hit_record_t *rec) {
    if ((a < 0 || a > 1) || (b < 0 || b > 1))
        return false;
    rec->u = a;
    rec->v = b;
    return true;
}

static bool         quad_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_QUAD) {
            printf("quad_hit() failed");
            return false;
    }
    my_quad_t *quad = (my_quad_t *)hittable;
    
    double  denom = vec3_dot(quad->normal, ray->dir);
    // printf("DENOM: %f \n", denom);
    // printf("X: %f \n", ray->dir.x);
    // printf("Y: %f \n", ray->dir.y);
    // printf("Z: %f \n", ray->dir.z);
    // fflush(stdout);
    // if ray is 0 or close to zero, then it never intersects. fabs() in case if ray intersects from opposite side of normal
    if (fabs(denom) < 1e-8)
        return false;
    // printf("quad->normal:%f %f %f\n", ray->dir.x, ray->dir.y, ray->dir.z);
    // printf("denom:%f\n", denom);
    // fflush(stdout);
    double t = (quad->D - vec3_dot(quad->normal, ray->orig)) / denom;
    // intersection is not within the valid segment of the ray
    if (t < tmin || t > tmax)
        return false;
 // Determine if the hit point lies within the planar shape using its plane coordinates.
    point3_t intersection = ray_at(ray->orig, ray->dir, t);
    point3_t planar_hitpt_vector = vec3_sub_return(intersection, quad->Q);
    double alpha = vec3_dot(planar_hitpt_vector, quad->u) / quad->u_len_squared;
    double beta = vec3_dot(planar_hitpt_vector, quad->v) / quad->v_len_squared;

    if (!is_interior(alpha, beta, rec)) {
        return false;
    }
    // printf("ray_orig:%f %f %f\n", ray->dir.x, ray->dir.y, ray->dir.z);
    rec->t = t;
    rec->p = intersection;
    rec->mat = quad->material;
    set_front_face(ray, &quad->normal, rec);

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

double              quad_pdf_value(const hittable_t *hittable, const point3_t *origin, const vec3_t *dir) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_QUAD) {
        printf("quad_pdf_value() failed");
        return 0.0;
    }
    my_quad_t *quad = (my_quad_t *)hittable;
    hit_record_t    rec;
    ray_t     r = ray(*origin, *dir, random_double_nolimits());

    if (!hittable_t_hit(hittable, &r, 0.001, INFINITY, &rec))
        return 0.0;
    
    double dist_squared = rec.t * rec.t * vec3_len_squared(*dir);
    double cosine = fabs(vec3_dot(*dir, rec.normal) / vec3_len(*dir));
    if (cosine < 1e-8)
        return 0.0;
    return dist_squared / (cosine * quad->area);
}

vec3_t              quad_random(const hittable_t *hittable, const point3_t *origin) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_QUAD) {
        printf("quad_random() failed");
        return vec3(0,0,0);
    }
    my_quad_t *quad = (my_quad_t *)hittable;
    // p = Q + v * r1 + u * r2;
    point3_t p = vec3_sum(vec3_sum(quad->Q, vec3_scaled_return(quad->u, random_double_nolimits())), vec3_scaled_return(quad->v, random_double_nolimits())); 
    vec3_t dir_to_light = vec3_sub_return(p, *origin);
    if (vec3_near_zero(&dir_to_light))
        return vec3(0,0,1);

    return vec3_normalize(dir_to_light);
}
