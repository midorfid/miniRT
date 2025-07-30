#include "../include/render/hittable_list.h"
#include "../include/scenes.h"
#include "../include/render/hittable.h"
#include "../include/textures/checker_pattern.h"
#include "../include/textures/perlin.h"
#include "../include/render/bvh.h"
#include "../include/render/instance.h"

hittable_list_t     *earth() {
    // auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    // auto earth_surface = mt_lambertian_new_with_colour(earth_texture);
    // auto globe = sphere_new(point3(0,0,0), 2, earth_surface);
    return NULL;
}

hittable_list_t         *quads() {
    hittable_list_t     *world;

    world = hittable_list_innit(6);

    // Materials
    material_t *left_red = mt_lambertian_new_with_colour(color_in(1.0, 0.2, 0.2)); 
    material_t *back_green = mt_lambertian_new_with_colour(color_in(0.2, 1.0, 0.2)); 
    material_t *right_blue = mt_lambertian_new_with_colour(color_in(0.2, 0.2, 1.0)); 
    material_t *upper_orange = mt_lambertian_new_with_colour(color_in(1.0, 0.5, 0.0)); 
    material_t *lower_teal = mt_lambertian_new_with_colour(color_in(0.2, 0.8, 0.8)); 

    material_t *light_material = diffuse_light_new_with_colour(color_in(15, 15, 15)); // Bright white light

    // Quads
    hittable_list_add(world, quad_new(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    hittable_list_add(world, quad_new(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    hittable_list_add(world, quad_new(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    hittable_list_add(world, quad_new(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    hittable_list_add(world, quad_new(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    hittable_list_add(world, quad_new(point3(0, 2.99, 2), vec3(1, 0, 0), vec3(0, 0, -1), light_material)); // Example position
    
    return world;
}

hittable_list_t     *cornell_smoke() {
    hittable_list_t *world = hittable_list_innit(8);

    material_t *red   = mt_lambertian_new_with_colour(color(.65, .05, .05));
    material_t *white = mt_lambertian_new_with_colour(color(.73, .73, .73));
    material_t *green = mt_lambertian_new_with_colour(color(.12, .45, .15));
    material_t *light = diffuse_light_new_with_colour(color(7, 7, 7));

    hittable_list_add(world, quad_new(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    hittable_list_add(world, quad_new(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    hittable_list_add(world, quad_new(point3(113,554,127), vec3(330,0,0), vec3(0,0,305), light));
    hittable_list_add(world, quad_new(point3(0,555,0), vec3(555,0,0), vec3(0,0,555), white));
    hittable_list_add(world, quad_new(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    hittable_list_add(world, quad_new(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    // hittable_t *box1 = box_new(point3(0,0,0), point3(165,330,165), white);
    // box1 = make_shared<rotate_y>(box1, 15);
    // box1 = make_shared<translate>(box1, vec3(265,0,295));

    // hittable_t *box2 = box_new(point3(0,0,0), point3(165,165,165), white);
    // box2 = make_shared<rotate_y>(box2, -18);
    // box2 = make_shared<translate>(box2, vec3(130,0,65));

    // hittable_list_add(world, const_medium_new_with_colour(box1, 0.01, color(0,0,0)));
    // hittable_list_add(world, const_medium_new_with_colour(box2, 0.01, color(1,1,1)));

    return world;
}

hittable_list_t     *final_scene(int image_width, int samples_per_pixel, int max_depth) {
    hittable_list_t *boxes1;
    material_t *ground = mt_lambertian_new_with_colour(color(0.48, 0.83, 0.53));
    int boxes_per_side = 20;
    boxes1 = hittable_list_innit(20*20);
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            double w = 100.0;
            double x0 = -1000.0 + i*w;
            double z0 = -1000.0 + j*w;
            double y0 = 0.0;
            double x1 = x0 + w;
            double y1 = random_double(1,101);
            double z1 = z0 + w;

            hittable_list_add(boxes1, box_new(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittable_list_t *world;
    // not sure about time
    hittable_list_add(world, bvh_node_new(boxes1, 0.0, 1.0));

    material_t *light = mt_lambertian_new_with_colour(color(7, 7, 7));
    hittable_list_add(world, quad_new(point3(123,554,147), vec3(300,0,0), vec3(0,0,265), light));

    point3_t center1 = point3(400, 400, 200);
    point3_t center2 = vec3_sum(center1, vec3(30,0,0));
    material_t *sphere_material = mt_lambertian_new_with_colour(color(0.7, 0.3, 0.1));
    hittable_list_add(world, mv_sphere_new(center1, center2, 0.0, 1.0, 50, sphere_material));

    hittable_list_add(world, sphere_new(point3(260, 150, 45), 50, mt_dielectric_new(1.5)));
    hittable_list_add(world, sphere_new(
        point3(0, 150, 145), 50, mt_metal_new(color(0.8, 0.8, 0.9), 1.0)
    ));

    hittable_t *boundary = sphere_new(point3(360,150,145), 70, mt_dielectric_new(1.5));
    hittable_list_add(world, boundary);
    hittable_list_add(world, const_medium_new_with_colour(boundary, 0.2, 0.4));
    boundary = sphere_new(point3(0,0,0), 5000, mt_dielectric_new(1.5));
    hittable_list_add(world, const_medium_new_with_colour(boundary, .0001, 0.1));

    // material_t *emat = mt_lambertian_new_with_colour(make_shared<image_texture>("earthmap.jpg"));
    // hittable_list_add(sphere_new(point3(400,200,400), 100, emat));
    material_t *pertext = mt_lambertian_new_with_tex(perlin_new(0.2));
    hittable_list_add(world, sphere_new(point3(220,280,300), 80, pertext));

    hittable_list_t *boxes2;
    material_t *white = mt_lambertian_new_with_colour(color(.73, .73, .73));
    int ns = 1000;
    boxes2 = hittable_list_innit(ns);
    for (int j = 0; j < ns; j++) {
        hittable_list_add(boxes2, sphere_new(point3(random_double(0.0, 165.0), random_double(0.0, 165.0), random_double(0.0, 165.0)), 10, white));
    }

    // hittable_list_add(make_shared<translate>(
    //     make_shared<rotate_y>(
    //         make_shared<bvh_node>(boxes2), 15),
    //         vec3(-100,270,395)
    //     )
    // );

    return world;
}

// world.add(sphere_new(point3(0,7,0), 2, difflight));
hittable_list_t     *simple_light() {
    hittable_list_t *world = hittable_list_innit(3);

    material_t *pertext = mt_lambertian_new_with_tex(perlin_new(4.0));
    material_t *difflight = diffuse_light_new_with_colour(color(4, 4, 4));

    hittable_list_add(world, quad_new(point3(3, 1, -2), vec3(2,0,0), vec3(0,2,0), difflight));
    hittable_list_add(world, sphere_new(point3(0,-1000,0), 1000, pertext));
    hittable_list_add(world, sphere_new(point3(0,2,0), 2, material_claim(pertext)));

    return world;
}

hittable_list_t     *perlin_spheres() {
    hittable_list_t *world = hittable_list_innit(2);

    material_t *pertext = mt_lambertian_new_with_tex(perlin_new(4.0));
    hittable_list_add(world, sphere_new(point3(0,-1000,0), 1000, pertext));
    hittable_list_add(world, sphere_new(point3(0,2,0), 2, material_claim(pertext)));

    return world;
}

hittable_list_t     *cornell_box_empty() {
    hittable_list_t *world = hittable_list_innit(6);

    material_t *red   = mt_lambertian_new_with_colour(color(.65, .05, .05));
    material_t *white = mt_lambertian_new_with_colour(color(.73, .73, .73));
    material_t *green = mt_lambertian_new_with_colour(color(.12, .45, .15));
    material_t *light = diffuse_light_new_with_colour(color(4, 4, 4));

    hittable_list_add(world, quad_new(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    hittable_list_add(world, quad_new(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    hittable_list_add(world, quad_new(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    hittable_list_add(world, quad_new(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    hittable_list_add(world, quad_new(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    hittable_list_add(world, quad_new(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    return world;
}

hittable_list_t         *cornell_box_standard(hittable_list_t **lights) {
    hittable_list_t *world = hittable_list_innit(8);
    *lights = hittable_list_innit(1);

    material_t *red   = mt_lambertian_new_with_colour(color(.65, .05, .05));
    material_t *white = mt_lambertian_new_with_colour(color(.73, .73, .73));
    material_t *green = mt_lambertian_new_with_colour(color(.12, .45, .15));
    material_t *light = diffuse_light_new_with_colour(color(4, 4, 4));

    hittable_list_add(*lights, quad_new(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), material_claim(light)));

    hittable_list_add(world, quad_new(point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    hittable_list_add(world, quad_new(point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    hittable_list_add(world, quad_new(point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    hittable_list_add(world, quad_new(point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), material_claim(white)));
    hittable_list_add(world, quad_new(point3(0,0,555), vec3(555,0,0), vec3(0,555,0), material_claim(white)));
    hittable_list_add(world, quad_new(point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), material_claim(light)));
    
    hittable_t *box1 = instance_new(box_new(point3(0,0,0), point3(165,330,165), material_claim(white)));
    // hittable_t *box1 = box_new(point3(130, 0, 65), point3(295, 165, 230), material_claim(white));
    instance_rotate_y(box1, -18);
    instance_translate(box1, vec3(265,0,295));
    hittable_list_add(world, box1);

    hittable_t *box2 = instance_new(box_new(point3(0,0,0), point3(165,165,165), material_claim(white)));
    // hittable_t *box2 = box_new(point3(265, 0, 295), point3(430, 330, 460), material_claim(white));
    instance_rotate_y(box2, 15);
    instance_translate(box2, vec3(130,0,65));
    hittable_list_add(world, box2);

    hittable_list_t *result = hittable_list_innit(1);
    hittable_list_add(result, bvh_node_new(world, 0.0, 1.0));

    return result;
}

hittable_list_t     *checkered_spheres() {
    hittable_list_t *world = hittable_list_innit(2);

    material_t *checker = mt_lambertian_new_with_tex(checker_pat_new(0.32, solid_colour_new(color(.2, .3, .1)), solid_colour_new(color(.9, .9, .9)))); 

    hittable_list_add(world, sphere_new(point3(0,-10, 0), 10, checker));
    hittable_list_add(world, sphere_new(point3(0, 10, 0), 10, material_claim(checker)));

    return world;
}

hittable_list_t         *touching_spheres() {
    double R = cos(PI/4);

    hittable_list_t *world = hittable_list_innit(2);

    material_t  *material_left = mt_lambertian_new_with_colour(color_in(0,0,1));
    material_t  *material_right = mt_lambertian_new_with_colour(color_in(1,0,0));

    hittable_list_add(world, sphere_new(vec3(-R,0,-1.0), R, material_left));
    hittable_list_add(world, sphere_new(vec3(R,0,-1.0), R, material_right));

    return world;
}

hittable_list_t         *five_spheres() {
    hittable_list_t *world = hittable_list_innit(5);
    
    material_t  *material_ground = mt_lambertian_new_with_colour(color_in(0.8, 0.8, 0.0));
    material_t  *material_center = mt_lambertian_new_with_colour(color_in(0.1, 0.2, 0.5));
    material_t  *material_left = mt_dielectric_new(1.50);
    material_t  *material_bubble = mt_dielectric_new(1.00 / 1.50);
    material_t  *material_right = mt_metal_new(color_in(0.8, 0.6, 0.2), 1.0);

    hittable_list_add(world, sphere_new(vec3(0.0,-100.5,-1.0), 100.0, material_ground));
    hittable_list_add(world, sphere_new(vec3(0.0,0.0,-1.2), 0.5, material_center));
    hittable_list_add(world, sphere_new(vec3(-1.0,0.0,-1.0), 0.5, material_left));
    hittable_list_add(world, sphere_new(vec3(-1.0,0.0,-1.0), 0.4, material_bubble));
    hittable_list_add(world, sphere_new(vec3(1.0,0.0,-1.0), 0.5, material_right));
    
    return world;
}

hittable_list_t     *bouncing_spheres() {
    hittable_list_t     *world = hittable_list_innit(500);

    material_t  *material_ground = mt_lambertian_new_with_colour(color_in(0.5, 0.5, 0.5));
    hittable_list_add(world, sphere_new(vec3(0.0,-1000.0, 0.0), 1000.0, material_ground));

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            double choose_mat = random_double_nolimits();
            point3_t center = point3(a + 0.9 * random_double_nolimits(), 0.2, b + 0.9 * random_double_nolimits());

            if (vec3_len(vec3_sub_return(center, point3(4, 0.2, 0))) > 0.9) {
                material_t *material = NULL;
                hittable_t *object_to_add = NULL;
                if (choose_mat < 0.8) {
                    // diffuse
                    color_t albedo = vec3_multi(vec3_random(), vec3_random());
                    material = mt_lambertian_new_with_colour(albedo);
                    point3_t center2 = (vec3_sum(center, vec3(0, random_double(0,.5), 0)));

                    object_to_add = mv_sphere_new(center, center2, 0, 1, 0.2, material);
                }
                else if (choose_mat < 0.95) {
                    // metal
                    color_t albedo2 = vec3_random_limits(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    material = mt_metal_new(albedo2, fuzz);

                    object_to_add = sphere_new(center, 0.2, material);
                }
                else {
                    // glass
                    material = mt_dielectric_new(1.5);

                    object_to_add = sphere_new(center, 0.2, material);
                }
                if (object_to_add != NULL)
                    hittable_list_add(world, object_to_add);
            }
        }
    }

    material_t *material1 = mt_dielectric_new(1.5);
    hittable_list_add(world, sphere_new(point3(0,1,0), 1.0, material1));

    material_t *material2 = mt_lambertian_new_with_colour(color_in(0.4, 0.2, 0.1));
    hittable_list_add(world, sphere_new(point3(-4,1,0), 1.0, material2));

    material_t *material3 = mt_metal_new(color_in(0.7,0.6,0.5), 0.0);
    hittable_list_add(world, sphere_new(point3(4,1,0), 1.0, material3));

    hittable_list_t *result = hittable_list_innit(1);
    hittable_list_add(result, bvh_node_new(world, 0.0, 1.0));
    return result;
}
