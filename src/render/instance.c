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
    matrix_default(new_instance->rotate_transform_m);
    matrix_default(new_instance->scale_transform_m);

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
    matrix_multi_by_matrix(&instance->rotate_transform_m[RAY], matrix_rotation_y(-angle));
    matrix_multi_by_matrix(&instance->rotate_transform_m[BB], matrix_rotation_y(angle));

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

static bool     instance_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_INSTANCE) {
        printf("instance_hit() failed");
        return false;
    }
    instance_t  *instance = (instance_t *)hittable;
    
    ray_t       offset_ray;
    // Firstly, move by offset
    offset_ray.orig = vec3_sub_return(ray->orig, instance->offset);
    // Then rotate and scale
    matrix3_t local_transform_m = instance->scale_transform_m[RAY];
    matrix_multi_by_matrix(&local_transform_m, instance->rotate_transform_m[RAY]);

    offset_ray.orig = vec3_multi_by_matrix(&offset_ray.orig, &local_transform_m);
    offset_ray.dir = vec3_multi_by_matrix(&offset_ray.dir, &local_transform_m);
    if (!hittable_t_hit(instance->hittable, &offset_ray, tmin, tmax, rec))
        return false;
    // matrix3_t forward_point_transform = instance->scale_transform_m[BB];
    // matrix_multi_by_matrix(&forward_point_transform, instance->rotate_transform_m[BB]);
    
    // rec->p = vec3_multi_by_matrix(&rec->p, &forward_point_transform);
    // vec3_add(&rec->p, instance->offset);
    
    // matrix3_t inversed_normal_transform = instance->scale_transform_m[RAY];
    // matrix_multi_by_matrix(&inversed_normal_transform, instance->rotate_transform_m[RAY]);

    // rec->normal = vec3_multi_by_matrix(&rec->normal, &inversed_normal_transform);
    
    rec->p = vec3_multi_by_matrix(&rec->p, &local_transform_m);
    vec3_add(&rec->p, instance->offset);
    matrix3_t inversed_scale = matrix_inverse_scale(&instance->scale_transform_m[BB]);
    matrix_multi_by_matrix(&inversed_scale, instance->rotate_transform_m[BB]);
    rec->normal = vec3_multi_by_matrix(&rec->normal, &inversed_scale);

    set_front_face(ray, &rec->normal, rec);

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
      //scale && rotate
    vec3_t min = hittable_bb.min;
    vec3_t max = hittable_bb.max;
    matrix3_t   local_matrix = instance->scale_transform_m[BB];
    matrix_multi_by_matrix(&local_matrix, instance->rotate_transform_m[BB]);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                point3_t p = vec3(i * hittable_bb.max.x + (1 - i) * hittable_bb.min.x,
                                j * hittable_bb.max.y + (1 - j) * hittable_bb.min.y,
                                k * hittable_bb.max.z + (1 - k) * hittable_bb.min.z);
                vec3_t test = vec3_multi_by_matrix(&p, &local_matrix);
                for (int c = 0; c < 3; ++c) {
                    min.components[c] = fmin(test.components[c], min.components[c]);
                    max.components[c] = fmax(test.components[c], max.components[c]);
                }
            }
        }
    }
    *out_bbox = aabb_two_points(vec3_sum(min, instance->offset), vec3_sum(max, instance->offset));
    // matrix3_t   local_matrix = instance->scale_transform_m[BB];
    // matrix_multi_by_matrix(&local_matrix, instance->rotate_transform_m[BB]);
// 
    // point3_t    first_corner = vec3(hittable_bb.min.x, hittable_bb.min.y, hittable_bb.min.z);
    // vec3_t      transformed_first = vec3_multi_by_matrix(&first_corner, &local_matrix);
// 
    // vec3_t min_transformed_box = transformed_first;
    // vec3_t max_transformed_box = transformed_first;
// 
    // for (int i = 0; i < 2; ++i) {
        // for (int j = 0; j < 2; ++j) {
            // for (int k = 0; k < 2; ++k) {
                // point3_t p = vec3(i ? hittable_bb.max.x : hittable_bb.min.x,
                                // j ? hittable_bb.max.y : hittable_bb.min.y,
                                // k ? hittable_bb.max.z : hittable_bb.min.z);
                // vec3_t test = vec3_multi_by_matrix(&p, &local_matrix);
                // for (int c = 0; c < 3; ++c) {
                    // min_transformed_box.components[c] = fmin(test.components[c], min_transformed_box.components[c]);
                    // max_transformed_box.components[c] = fmax(test.components[c], max_transformed_box.components[c]);
                // }
            // }
        // }
    // }
    //translate
    // *out_bbox = aabb_two_points(vec3_sum(min_transformed_box, instance->offset), vec3_sum(max_transformed_box, instance->offset));

    return true;
}

void            instance_uniform_scale(hittable_t *hittable, double uniform_scale) {
    instance_scale(hittable, vec3(uniform_scale, uniform_scale, uniform_scale));
}

void            instance_scale(hittable_t *hittable, vec3_t scale_vector) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_INSTANCE) {
            printf("instance_scale() failed");
            return ;
    }
    instance_t *instance = (instance_t *)hittable;

    matrix_multi_by_matrix(&instance->scale_transform_m[RAY] , matrix_scale(scale_vector));
    matrix_multi_by_matrix(&instance->scale_transform_m[BB], matrix_scale(scale_vector));
}