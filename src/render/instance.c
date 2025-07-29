#include "../../include/render/instance.h"

void            instance_delete(hittable_t *hittable) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_INSTANCE) {
        printf("instance_delete() failed");
        return ;
    }
    instance_t  *todelete = (instance_t *)hittable;

    hittable_delete(todelete->hittable);
    free(todelete);
}

hittable_t      *instance_new(hittable_t *hittable) {
    instance_t  *new_instance = calloc(1, sizeof(instance_t));
    if (new_instance == NULL) {
        printf("instance_new() failed");
        return NULL;
    }
    new_instance->hittable = hittable;
    hittable_innit(&new_instance->base, HITTABLE_TYPE_INSTANCE, instance_hit, instance_bb, instance_delete);
    new_instance->offset = vec3(0.0, 0.0, 0.0);
    new_instance->transform_matrix_bb = matrix_default();
    new_instance->transform_matrix_ray = matrix_default();

    return (hittable_t *)new_instance;
}

void            instance_translate(hittable_t *hittable, vec3_t offset) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_INSTANCE) {
            printf("instance_translate() failed");
            return ;
    }
    instance_t *instance = (instance_t *)hittable;
    vec3_add(&instance->offset, offset);
}

void        instance_rotate_y(hittable_t *hittable, double angle) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_INSTANCE) {
            printf("instance_rotate_y() failed");
            return ;
    }
    instance_t  *instance = (instance_t *)hittable;

    matrix_multi_by_matrix(&instance->transform_matrix_ray, matrix_rotation_y(-DEG_TO_RAD(angle)));
    matrix_multi_by_matrix(&instance->transform_matrix_bb, matrix_rotation_y(DEG_TO_RAD(angle)));
}

vec3_t          vec3_multi_by_matrix(const vec3_t *a, const matrix3_t *matrix) {
    vec3_t  new;
    for (int i = 0; i < 3; ++i) {
        new.components[i] = a->components[0] * matrix->matrix[0][i] +
                        a->components[1] * matrix->matrix[1][i] +
                        a->components[2] * matrix->matrix[2][i]; 
    }
    return new;
}

static bool     instance_hit(const hittable_t *hittable, const ray_t *r, double tmin, double tmax, hit_record_t *rec) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_INSTANCE) {
        printf("instance_hit() failed");
        return false;
    }
    instance_t  *instance = (instance_t *)hittable;
    
    ray_t       transformed_ray;

    transformed_ray = ray(vec3_sub_return(r->orig, instance->offset), r->dir, r->time);

    transformed_ray.orig = vec3_multi_by_matrix(&transformed_ray.orig, &instance->transform_matrix_ray);
    transformed_ray.dir = vec3_multi_by_matrix(&transformed_ray.dir, &instance->transform_matrix_ray);

    if (!hittable_t_hit(instance->hittable, &transformed_ray, tmin, tmax, rec))
        return false;
    rec->p = vec3_multi_by_matrix(&rec->p, &instance->transform_matrix_bb);
    vec3_add(&rec->p, instance->offset);
    
    vec3_t new_normal = vec3_multi_by_matrix(&rec->normal, &instance->transform_matrix_bb);
    
    set_front_face(r, &new_normal, rec);

    return true;
}

static bool     instance_bb(const hittable_t *hittable, double time0, double time1, aabb_t *out_bbox) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_INSTANCE) {
        printf("instance_bb() failed");
        return false;
    }
    instance_t  *instance = (instance_t *)hittable;
    
    aabb_t  hittable_bb;
    if (!hittable_t_bb(instance->hittable, time0, time1, &hittable_bb)) {
        return false;
    }
    matrix3_t   local_matrix = instance->transform_matrix_bb;

    point3_t    first_corner = vec3(hittable_bb.min.x, hittable_bb.min.y, hittable_bb.min.z);
    vec3_t      transformed_first = vec3_multi_by_matrix(&first_corner, &local_matrix);

    vec3_t min_transformed_box = transformed_first;
    vec3_t max_transformed_box = transformed_first;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                point3_t p = vec3(i ? hittable_bb.max.x : hittable_bb.min.x,
                                j ? hittable_bb.max.y : hittable_bb.min.y,
                                k ? hittable_bb.max.z : hittable_bb.min.z);
                vec3_t test = vec3_multi_by_matrix(&p, &local_matrix);
                for (int c = 0; c < 3; ++c) {
                    min_transformed_box.components[c] = fmin(test.components[c], min_transformed_box.components[c]);
                    max_transformed_box.components[c] = fmax(test.components[c], max_transformed_box.components[c]);
                }
            }
        }
    }
    //translate
    *out_bbox = aabb_two_points(vec3_sum(min_transformed_box, instance->offset), vec3_sum(max_transformed_box, instance->offset));

    return true;
}

// void            instance_uniform_scale(hittable_t *hittable, double uniform_scale) {
//     instance_scale(hittable, vec3(uniform_scale, uniform_scale, uniform_scale));
// }

// void            instance_scale(hittable_t *hittable, vec3_t scale_vector) {
//     if (hittable == NULL || hittable->type != HITTABLE_TYPE_INSTANCE) {
//             printf("instance_scale() failed");
//             return ;
//     }
//     instance_t *instance = (instance_t *)hittable;

//     matrix_multi_by_matrix(&instance->scale_transform_m[RAY] , matrix_scale(scale_vector));
//     matrix_multi_by_matrix(&instance->scale_transform_m[BB], matrix_scale(scale_vector));
// }