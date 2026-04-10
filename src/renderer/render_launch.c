#include "renderer/render_launch.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "pthread.h"

#define CHUNK 128

int     save_image_to_png(mlx_image_t *image, const char *filename);

static void     render_worker_complete(int status, void *args)
{
    render_context_t    *worker_args = args;

    pthread_mutex_lock(worker_args->process_mutex);

    int old_processed   = atomic_fetch_add(worker_args->processed_chunks, 1);
    int new_processed   = old_processed + 1;
    int old_pct         = 100 * old_processed / worker_args->total_chunks;
    int new_pct         = 100 * new_processed / worker_args->total_chunks;

    if (new_pct != old_pct)
    {
        fprintf(stderr, "\rProgress: %d/%d chunks (%3d%%)",
            new_processed, worker_args->total_chunks, new_pct);
        fflush(stderr);
    }
    pthread_mutex_unlock(worker_args->process_mutex);
    free(worker_args->chunk_buffer);
    free(worker_args);
}

static void     render_pixel_chunk(void *param)
{
    render_context_t    *render = (render_context_t *)param;

    rt_random_seed(render->random_seed);
    for (int i = render->height + render->y_start - 1; i >= render->y_start; --i)
    {
        for (int j = render->x_start; j < render->x_start + render->width; ++j)
        {
            vec3_t color = vec3(0.0, 0.0, 0.0);
            for (int s_i = 0; s_i < render->camera.sqrt_spp; ++s_i)
            {
                for (int s_j = 0; s_j < render->camera.sqrt_spp; ++s_j)
                {
                    ray_t r = get_ray(i, j, s_i, s_j, &render->render_p,
                        render->camera.camera_center, &render->camera.lens,
                        render->camera.rec1p_sqrt_spp);
                    color = vec3_sum(color, ray_color(&r, render->world,
                        render->lights, render->camera.max_depth, render->render_mode));
                }
            }
            color = vec3_scaled_return(color, render->camera.pixel_sample_scale);
            mlx_put_pixel(render->mlx_image, j, i, return_color(color));
        }
    }
}

void    render_launch(render_context_t *render, uint64_t initial_seed)
{
    int             num_threads     = (int)sysconf(_SC_NPROCESSORS_ONLN);
    pthread_mutex_t *process_mutex  = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(process_mutex, NULL);
    thread_pool_t   *pool           = thread_pool_init(num_threads);

    _Atomic int processed_chunks    = 0;
    int chunks_x                    = (int)ceil(render->image.image_width  / (double)CHUNK);
    int chunks_y            = (int)ceil(render->image.image_height / (double)CHUNK);
    int total_chunks        = chunks_x * chunks_y;

    clock_t start = clock();
    printf("Rendering...\n");

    for (int i = 0; i < chunks_y; ++i)
    {
        for (int j = 0; j < chunks_x; ++j)
        {
            render_context_t *arg = malloc(sizeof(render_context_t));
            if (arg == NULL)
            {
                fprintf(stderr, "render_launch: chunk alloc failed\n");
                return ;
            }
            *arg                    = *render;
            arg->process_mutex      = process_mutex;
            arg->processed_chunks   = (_Atomic int *)&processed_chunks;
            arg->total_chunks       = total_chunks;
            arg->random_seed        = initial_seed + ((uint64_t)i << 32 | (uint64_t)j);
            arg->x_start            = j * CHUNK;
            arg->y_start            = i * CHUNK;
            arg->width  = render->image.image_width  - j * CHUNK < CHUNK
                ? render->image.image_width  - j * CHUNK : CHUNK;
            arg->height = render->image.image_height - i * CHUNK < CHUNK
                ? render->image.image_height - i * CHUNK : CHUNK;
            arg->chunk_buffer = malloc(arg->width * arg->height * sizeof(vec3_t));
            if (arg->chunk_buffer == NULL)
            {
                fprintf(stderr, "render_launch: chunk buffer alloc failed\n");
                free(arg);
                return ;
            }
            schedule_work(pool, render_pixel_chunk, arg, render_worker_complete);
        }
    }

    while (processed_chunks < total_chunks)
        usleep(100000);

    fprintf(stderr, "\n");
    printf("Rendering complete in %.2f seconds.\n",
        (double)(clock() - start) / CLOCKS_PER_SEC);

    if (!save_image_to_png(render->mlx_image, "output.png"))
        fprintf(stderr, "Failed to save the output image.\n");

    thread_pool_destroy(pool);
    pthread_mutex_destroy(process_mutex);
    free(process_mutex);
}
