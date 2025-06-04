
    int                 samples_per_pixel = 10;
    int                 max_depth = 50;
    point3_t            lookfrom = point3(13,2,3);
    point3_t            lookat = point3(0,0,0);
    vec3_t              vup = vec3(0,1,0);

    lens.defocus_angle = 0.6; // Variation angle of rays through each pixel
    double              focus_dist = 10.0; // Distance from camera lookfrom point to plane of perfect focus
    double              vfov = 20;


hittable_list_t     *world = hittable_list_innit(500);

    material_t  *material_ground = mt_lambertian_new(color_in(0.5, 0.5, 0.5));
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
                    material = mt_lambertian_new(albedo);
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

    material_t *material2 = mt_lambertian_new(color_in(0.4, 0.2, 0.1));
    hittable_list_add(world, sphere_new(point3(-4,1,0), 1.0, material2));

    material_t *material3 = mt_metal_new(color_in(0.7,0.6,0.5), 0.0);
    hittable_list_add(world, sphere_new(point3(4,1,0), 1.0, material3));

    hittable_list_t *result = hittable_list_innit(1);
    hittable_list_add(result, bvh_node_new(world, 0.0, 1.0));
