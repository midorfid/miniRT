#include "../include/render_context.h"
#include <pthread.h>
#include "../include/stb_image_write.h"
#include <unistd.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

#define CHUNK 32

int save_image_to_png(mlx_image_t* image, const char* filename);

void    render_pixel_chunk(void *param) {
    render_context_t *render = (render_context_t *)param;
    for(int i = render->height + render->y_start - 1; i >= render->y_start; --i) {
        for (int j = render->x_start; j < render->x_start + render->width; ++j) {
            vec3_t color = vec3(0.0, 0.0, 0.0);
            
            // printf("  Processing pixel (x:%d, y:%d)...\n", j, i);
            // fflush(stdout); // This forces the output to print immediately
            
            for (int s_i = 0; s_i < render->camera.sqrt_spp; ++s_i) {
                for (int s_j = 0; s_j < render->camera.sqrt_spp; ++s_j) {
                    ray_t r = get_ray(i, j, s_i, s_j, &render->render_p, render->camera.camera_center, &render->camera.lens, render->camera.rec1p_sqrt_spp); 
                    color = vec3_sum(color, ray_color(&r, render->world, render->camera.max_depth));
                }
            }
            color = vec3_scaled_return(color, render->camera.pixel_sample_scale);
            mlx_put_pixel(render->mlx_image, j, i, return_color(color));
        }
    }
}

static void            render_worker_complete(int status, void *args);

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

    render = calloc(1, sizeof(render_context_t));
    if (render == NULL) {
        printf("render context new failed");
        return -1;
    }

    // choose scene
    // TODO render_context_new and different scenes should have same vars values
    scene_id_t scene_id = SCENE_CORNELL_BOX_EMPTY;
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
            render->image.aspect_ratio      = 1.0;
            render->image.image_width       = 600;
            render->camera.samples_per_pixel = 20;
            render->camera.max_depth         = 50;

            render->pov.vfov        = 40;
            render->camera.lookfrom = point3(0, 0, 0.1);
            render->camera.lookat   = point3(0, 0, -1);
            render->camera.vup      = vec3(0,1,0);

            render->world = touching_spheres();
            break;

        case SCENE_FIVE_SPHERES:
            render->image.aspect_ratio      = 16.0 / 9.0;
            render->image.image_width       = 400;
            render->camera.samples_per_pixel = 10;
            render->camera.max_depth         = 50;

            render->pov.vfov        = 90;
            render->camera.lookfrom = point3(-2, 2, 1);
            render->camera.lookat   = point3(0, 0, -1);
            render->camera.vup      = vec3(0,1,0);

            render->world = five_spheres();
            break;

        case SCENE_QUAD:
            render->image.aspect_ratio      = 1.0;
            render->image.image_width       = 400;
            render->camera.samples_per_pixel = 10;
            render->camera.max_depth         = 50;

            render->pov.vfov        = 80;
            render->camera.lookfrom = point3(0,0,9);
            render->camera.lookat   = point3(0,0,0);
            render->camera.vup      = vec3(0,1,0);

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
            render->camera.samples_per_pixel = 10;
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
    render = render_context_new(render, mlx, image);
    // init threads
    
    int             num_threads = 1;
    pthread_mutex_t      *process_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(process_mutex, NULL);
    thread_pool_t   *pool = thread_pool_init(num_threads);
    // chunks
    int             processed_chunks = 0;
    int             total_chunks = (int)(ceil(render->image.image_height / (double)CHUNK)) * (int)(ceil(render->image.image_width / (double)CHUNK));

    clock_t start = clock();
    printf("Rendering...\n");
    for (int i = 0; i < ceil(render->image.image_height / (double)CHUNK); ++i) {
        for (int j = 0; j < ceil(render->image.image_width / (double)CHUNK); ++j) {
            render_context_t *arg = malloc(sizeof(render_context_t));
            if (arg == NULL) {
                printf("main render failed");
                return(EXIT_FAILURE);
            }
            arg->mlx = render->mlx;
            arg->mlx_image = render->mlx_image;
            arg->image = render->image;
            arg->pov = render->pov;
            arg->camera = render->camera;
            arg->render_p = render->render_p;
            arg->world = render->world;
            arg->process_mutex = process_mutex;
            arg->processed_chunks = &processed_chunks;
            arg->total_chunks = total_chunks;
            arg->x_start = j * CHUNK;
            arg->y_start = i * CHUNK;
            arg->width = render->image.image_width - j * CHUNK < CHUNK ? render->image.image_width - j * CHUNK : CHUNK;
            arg->height = render->image.image_height - i * CHUNK < CHUNK ? render->image.image_height - i * CHUNK : CHUNK;
            schedule_work(pool, render_pixel_chunk, arg, render_worker_complete);
        }
    }

    // Wait for all chunks to finish rendering
    // This is a simple "busy-wait" loop. It checks if all tasks are done.
    printf("processed: %d, total: %d\n", processed_chunks, total_chunks);
    while (processed_chunks < total_chunks)
    {
        // Sleep for a short duration to avoid maxing out the CPU.
        usleep(10000); // Sleep for 10 milliseconds.
    }
    
    // Print the final progress bar line.
    fprintf(stderr, "\rProgress: %d/%d chunks (%3d%%)\n", total_chunks, total_chunks, 100);

    printf("Rendering complete in %.2f seconds.\n", (double)(clock() - start) / CLOCKS_PER_SEC);

    // Save the final image to "output.png"
    // Make sure your save_image_to_png function is defined elsewhere.
    if (!save_image_to_png(image, "output.png")) {
        fprintf(stderr, "Failed to save the output image.\n");
    }

    // --- Clean up ---
    // Destroy the thread pool first to ensure all threads are stopped.
    thread_pool_destroy(pool);

    // Now destroy the mutex.
    pthread_mutex_destroy(process_mutex);
    free(process_mutex); // You were missing this free!

    // Finally, terminate MLX42.
    mlx_terminate(mlx);

    printf("Program finished and resources cleaned up.\n");
    
    return(0);
}

static void            render_worker_complete(int status, void *args) {
    render_context_t *worker_args = args;

    pthread_mutex_lock(worker_args->process_mutex);

    int old_processed_chunks = *worker_args->processed_chunks;
    *worker_args->processed_chunks += 1;
    int current_processed_chunks = *worker_args->processed_chunks;
    int old_percentage = 100 * old_processed_chunks / worker_args->total_chunks;
    int current_percentage = 100 * current_processed_chunks / worker_args->total_chunks;

    if (current_percentage != old_percentage)
    {
        fprintf(stderr, "\rProgress: %d/%d chunks (%3d%%)", current_processed_chunks, worker_args->total_chunks,
                current_percentage);
        fflush(stderr);
    }
    
    pthread_mutex_unlock(worker_args->process_mutex);

    free(worker_args);
}
