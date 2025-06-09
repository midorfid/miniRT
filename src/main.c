#include "../include/render_context.h"
#include <pthread.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

void    *render_pixel_chunk(void *param) {
    thread_data_t   *data = (thread_data_t *)param;
    render_context_t *render = data->context;
    
    for(int i = data->start_row; i < data->end_row; i += (data->thread_id + 1)) {
        for (int j = 0; j < render->image.image_width; ++j) {
            vec3_t color = vec3(0.0, 0.0, 0.0);
            for (int sample = 0;sample < render->camera.samples_per_pixel;++sample) {
                ray_t r = get_ray(i, j, &render->render_p, render->camera.camera_center, &render->camera.lens); 
                color = vec3_sum(color, ray_color(&r, render->world, render->camera.max_depth));
            }
            color = vec3_scaled_return(color, render->camera.pixel_sample_scale);
            mlx_put_pixel(render->mlx_image, j, i, return_color(color));
        }
    }
    // if (data->start_row >= render->image.image_height) {
        // render->render_complete = true;
        // printf("rendering complete");
    // }
    return NULL;
}

int main(void) {

    // random generator seed set up
    uint64_t initial_seed = (uint64_t)time(NULL);
    rt_random_seed(initial_seed);

    // MLX42

    mlx_t               *mlx;
    mlx_image_t         *image;
    

    // Render
    mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "miniRT", true);
    if (!mlx) {
        fprintf(stderr, "Error initializing MLX42: %s\n", mlx_strerror(mlx_errno));
        return(1);
    }
    image = mlx_new_image(mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!image || mlx_image_to_window(mlx, image, 0, 0) < 0)
        return(1);

    render_context_t    *render; 

    render = render_context_new(mlx, image);


    // choose scene
    scene_id_t scene_id = SCENE_QUAD;
    switch(scene_id) {
        case SCENE_BOUNCING_SPHERES:
            render->camera.samples_per_pixel = 10;
            render->camera.max_depth = 50;
            render->camera.lookfrom = point3(13,2,3);
            render->camera.lookat = point3(0,0,0);
            render->camera.vup = vec3(0,1,0);
            
            render->camera.lens.defocus_angle = 0.6; // Variation angle of rays through each pixel
            render->camera.lens.focus_dist = 10.0; // Distance from camera lookfrom point to plane of perfect focus
            render->pov.vfov = 20;
            render->world = bouncing_spheres();
            break;

        case SCENE_CORNELL_BOX_EMPTY:
            render->image.aspect_ratio      = 1.0;
            render->image.image_width       = 600;
            render->camera.samples_per_pixel = 200;
            render->camera.max_depth         = 50;

            render->pov.vfov        = 40;
            render->camera.lookfrom = point3(278, 278, -800);
            render->camera.lookat   = point3(278, 278, 0);
            render->camera.vup      = vec3(0,1,0);

            render->camera.lens.defocus_angle = 0;
            render->world = cornell_box_empty();
            break;
        
        case SCENE_CORNELL_BOX_STANDARD:
            render->image.aspect_ratio      = 1.0;
            render->image.image_width       = 600;
            render->camera.samples_per_pixel = 200;
            render->camera.max_depth         = 50;

            render->pov.vfov        = 40;
            render->camera.lookfrom = point3(278, 278, -800);
            render->camera.lookat   = point3(278, 278, 0);
            render->camera.vup      = vec3(0,1,0);

            render->camera.lens.defocus_angle = 0;
            render->world = cornell_box_standard();
            break;

        case SCENE_TOUCHING_SPHERES:
            render->world = touching_spheres();
            break;

        case SCENE_QUAD:
            render->image.aspect_ratio      = 1.0;
            render->image.image_width       = 400;
            render->camera.samples_per_pixel = 100;
            render->camera.max_depth         = 50;

            render->pov.vfov        = 80;
            render->camera.lookfrom = point3(0,0,9);
            render->camera.lookat   = point3(0,0,0);
            render->camera.vup      = vec3(0,1,0);

            render->camera.lens.defocus_angle = 0;
            render->world = quads();
            break;

        case SCENE_TWO_CHECKERED_SPHERES:
            render->image.aspect_ratio      = 16.0 / 9.0;
            render->image.image_width       = 400;
            render->image.image_height       = 400;
            render->camera.samples_per_pixel = 10;
            render->camera.max_depth         = 50;

            render->pov.vfov        = 20;
            render->camera.lookfrom = point3(13,2,3);
            render->camera.lookat   = point3(0,0,0);
            render->camera.vup      = vec3(0,1,0);

            render->camera.lens.defocus_angle = 0;
            render->world = checkered_spheres();
            break;

        case SCENE_PERLIN_SPHERES:
            render->image.aspect_ratio      = 16.0 / 9.0;
            render->image.image_width       = 400;
            render->camera.samples_per_pixel = 100;
            render->camera.max_depth         = 50;

            render->pov.vfov        = 20;
            render->camera.lookfrom = point3(13,2,3);
            render->camera.lookat   = point3(0,0,0);
            render->camera.vup      = vec3(0,1,0);

            render->camera.lens.defocus_angle = 0;
            render->world = perlin_spheres();
            break;

        case SCENE_CORNELL_FOG:
            render->image.aspect_ratio      = 1.0;
            render->image.image_width       = 600;
            render->camera.samples_per_pixel = 200;
            render->camera.max_depth         = 50;

            render->pov.vfov        = 40;
            render->camera.lookfrom = point3(278, 278, -800);
            render->camera.lookat   = point3(278, 278, 0);
            render->camera.vup      = vec3(0,1,0);

            render->camera.lens.defocus_angle = 0;
            render->world = cornell_smoke();
            break;

        case SCENE_SIMPLE_LIGHT:
            render->image.aspect_ratio      = 16.0 / 9.0;
            render->image.image_width       = 400;
            render->camera.samples_per_pixel = 100;
            render->camera.max_depth         = 50;

            render->pov.vfov     = 20;
            render->camera.lookfrom = point3(26,3,6);
            render->camera.lookat   = point3(0,2,0);
            render->camera.vup      = vec3(0,1,0);

            render->camera.lens.defocus_angle = 0;
            render->world = simple_light();
            break;
        
        case SCENE_EARTH:
            render->image.aspect_ratio      = 16.0 / 9.0;
            render->image.image_width       = 400;
            render->camera.samples_per_pixel = 100;
            render->camera.max_depth         = 50;

            render->pov.vfov     = 20;
            render->camera.lookfrom = point3(0,0,12);
            render->camera.lookat   = point3(0,0,0);
            render->camera.vup      = vec3(0,1,0);

            render->camera.lens.defocus_angle = 0;
            render->world = earth();
            break;

        case SCENE_FINAL:
            render->image.aspect_ratio      = 1.0;

            render->pov.vfov     = 40;
            render->camera.lookfrom = point3(478, 278, -600);
            render->camera.lookat   = point3(278, 278, 0);
            render->camera.vup      = vec3(0,1,0);

            render->camera.lens.defocus_angle = 0;
            render->world = final_scene(400, 250, 4);
            break;

        case SCENE_NONE:
            // TODO print to log
            return EXIT_FAILURE;
    }

    // init threads
    
    int             num_threads = 1;
    
    pthread_t       thread_ids[num_threads];
    thread_data_t   thread_data[num_threads];
    
    int rows_per_thread = render->image.image_height / num_threads;
    
    clock_t start = clock();
    printf("Rendering...\n");
    for (int i = 0; i < num_threads; ++i) {
        thread_data[i].thread_id = i;
        thread_data[i].context = render;
        pthread_create(&thread_ids[i], NULL, render_pixel_chunk, &thread_data[i]);
    }
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(thread_ids[i], NULL);
    }
    printf("Rendering complete in %.2f\n", (double)(clock() - start) / CLOCKS_PER_SEC);
    // mlx_close_hook(mlx, &esc_exit, NULL);
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return(0);
}
