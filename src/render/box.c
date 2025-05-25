#include "../../include/render/box.h"

void    box_delete(hittable_t *hittable) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_BOX) {
        printf("box_delete() failed");
        return ;
    }
    my_box_t *box = (my_box_t *)hittable;
    
    delete_hittable_list(box->sides);
    free(box);
}

bool    box_hit(const hittable_t *hittable, const ray_t *ray, double tmin, double tmax, hit_record_t *rec) {
        if (hittable == NULL || hittable->type != HITTABLE_TYPE_BOX) {
        printf("box_hit() failed");
        return false;
    }
    my_box_t *box = (my_box_t *)hittable;

    return hittable_list_hit_test(ray, box->sides, tmin, tmax, rec);
}

bool    box_bb(const hittable_t *hittable, double time0, double time1, aabb_t *bbox) {
    if (hittable == NULL || hittable->type != HITTABLE_TYPE_BOX) {
        printf("box_bb() failed");
        return false;
    }
    my_box_t *box = (my_box_t *)hittable;
    *bbox = aabb_two_points(box->min, box->max);

    return true;
}

static hittable_list_t     *box_sides_init(point3_t a, point3_t b, material_t *material) {
    hittable_list_t *sides = hittable_list_innit(6);
    
    point3_t min = vec3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    point3_t max = vec3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    vec3_t  dx = vec3((max.x, min.x), 0.0, 0.0);
    vec3_t  dy = vec3(0.0, (max.y, min.y), 0.0);
    vec3_t  dz = vec3(0.0, 0.0, (max.z, min.z));

    hittable_list_add(sides, quad_new(vec3(min.x, min.y, max.z), dx, dy, material)); // front
    hittable_list_add(sides, quad_new(vec3(max.x, min.y, max.z), vec3_negative(&dz), dy, material)); // right side
    hittable_list_add(sides, quad_new(vec3(min.x, min.y, min.z), dz, dy, material)); // left side
    hittable_list_add(sides, quad_new(vec3(max.x, min.y, min.z), vec3_negative(&dx), dy, material)); // back
    hittable_list_add(sides, quad_new(vec3(min.x, min.y, min.z), dx, dz, material)); // bottom
    hittable_list_add(sides, quad_new(vec3(min.x, max.y, max.z), dx, vec3_negative(&dz), material)); // top

    return sides;
}

static my_box_t        box_innit(point3_t a, point3_t b, material_t *material) {
    my_box_t    box = 
    {
        .min = a,
        .max = b,
    };
    box.sides = box_sides_init(a, b, material); 

    hittable_innit(&box.base, HITTABLE_TYPE_BOX, box_hit, box_bb, box_delete);
    return box;
}

hittable_t      *box_new(point3_t a, point3_t b, material_t *material) {
    my_box_t    *new_box = calloc(1, sizeof(my_box_t));
    if (new_box == NULL) {
        printf("box_new() failed");
        return NULL;
    }
    *new_box = box_innit(a, b, material);
    return (hittable_t *)new_box;
}
