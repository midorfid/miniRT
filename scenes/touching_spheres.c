    TOuching spheres
    double R = cos(PI/4);

    material_t  *material_left = mt_lambertian_new(color_in(0,0,1));
    material_t  *material_right = mt_lambertian_new(color_in(1,0,0));

    hittable_list_add(world, sphere_new(vec3(-R,0,-1.0), R, material_left));
    hittable_list_add(world, sphere_new(vec3(R,0,-1.0), R, material_right));

    material_t  *material_ground = mt_lambertian_new(color_in(0.8, 0.8, 0.0));
    material_t  *material_center = mt_lambertian_new(color_in(0.1, 0.2, 0.5));
    material_t  *material_left = mt_dielectric_new(1.50);
    material_t  *material_bubble = mt_dielectric_new(1.00 / 1.50);
    material_t  *material_right = mt_metal_new(color_in(0.8, 0.6, 0.2), 1.0);

    hittable_list_add(world, sphere_new(vec3(0.0,-100.5,-1.0), 100.0, material_ground));
    hittable_list_add(world, sphere_new(vec3(0.0,0.0,-1.2), 0.5, material_center));
    hittable_list_add(world, sphere_new(vec3(-1.0,0.0,-1.0), 0.5, material_left));
    hittable_list_add(world, sphere_new(vec3(-1.0,0.0,-1.0), 0.4, material_bubble));
    hittable_list_add(world, sphere_new(vec3(1.0,0.0,-1.0), 0.5, material_right));
