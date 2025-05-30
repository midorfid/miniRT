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
    new_instance->rotate_transform_m = matrix_default();
    new_instance->scale_transform_m = matrix_default();

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
    matrix_multi_by_matrix(&instance->transform_m[RAY], matrix_rotation_y(-angle));
    matrix_multi_by_matrix(&instance->transform_m[BB], matrix_rotation_y(angle));
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
    point3_t offset_orig = vec3_sub_return(ray->orig, instance->offset);
    // Then rotate
    offset_orig = vec3_multi_by_matrix(&offset_orig, &instance->transform_m[RAY]);
    vec3_t offset_dir = vec3_multi_by_matrix(&ray->dir, &instance->transform_m[RAY]);
    if (!hittable_t_hit(instance->hittable, ray, tmin, tmax, rec))
        return false;
    // if hits change the intersection point from object space to world space by -theta rotation
    rec->p = vec3_multi_by_matrix(&rec->p, &instance->transform_m[BB]);
    vec3_add(&rec->p, instance->offset);
    rec->normal = vec3_multi_by_matrix(&rec->normal, &instance->transform_m[BB]);
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
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                point3_t p = vec3(i * hittable_bb.max.x + (1 - i) * hittable_bb.min.x,
                                j * hittable_bb.max.y + (1 - j) * hittable_bb.min.y,
                                k * hittable_bb.max.z + (1 - k) * hittable_bb.min.z);
                vec3_t test = vec3_multi_by_matrix(&p, &instance->transform_m[BB]);
                for (int c = 0; c < 3; ++c) {
                    min.components[c] = fmin(test.components[c], min.components[c]);
                    max.components[c] = fmax(test.components[c], max.components[c]);
                }
            }
        }
    }
    //translate
    *out_bbox = aabb_two_points(vec3_sum(min, instance->offset), vec3_sum(max, instance->offset));

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

    matrix_multi_by_matrix(&instance->transform_m[RAY], matrix_scale(scale_vector));
    matrix_multi_by_matrix(&instance->transform_m[BB], matrix_scale(scale_vector));
}