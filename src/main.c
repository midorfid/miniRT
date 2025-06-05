#include "../include/render_context.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

void    render_pixel_row_hook(void *param) {
    render_context_t *render = (render_context_t *)param;
    
    for (render->current_j = 0; render->current_j < render->image.image_width; render->current_j++) {
        vec3_t color = vec3(0.0, 0.0, 0.0);
        for (int sample = 0;sample < render->.camera.samples_per_pixel;++sample) {
            ray_t r = get_ray(render->current_i, render->current_j, &render->render_p, render->camera.camera_center, &render->camera.lens); 
            color = vec3_sum(color, ray_color(&r, world, render->camera.max_depth));
        }
        color = vec3_scaled_return(color, render->camera.pixel_sample_scale);
        mlx_put_pixel(render->mlx_image, render->current_j, render->current_i, return_color(color));
    }
    if (render->current_i >= render->image.image_height) {
        render->render_complete = true;
        printf("rendering complete");
    }
    render->current_i++;
}

int main(void) {

    // random generator seed set up
    uint64_t initial_seed = (uint64_t)time(NULL);
    rt_random_seed(initial_seed);

    // MLX42

    mlx_t               *mlx;
    mlx_image_t         *image;
    
    render_context_t    *render; 

    render = render_context_new(mlx, image);

    // choose scene
    scene_id = SCENE_TWO_CHECKERED_SPHERES;
    switch(scene_id) {
        case SCENE_BOUNCING_SPHERES:

            break;

        case SCENE_CORNELL_BOX:

            break;
        
        case SCENE_TOUCHING_SPHERES:

            break;

        case SCENE_QUAD:

            break;

        case SCENE_TWO_CHECKERED_SPHERES:

            break;

        case SCENE_CORNELL_FOG:

            break;

        case SCENE_SIMPLE_LIGHT:

            break;

        case SCENE_SIMPLE_LIGHT:

            break;

        case SCENE_EARTH:

            break;

        case SCENE_FINAL:

            break;

        case SCENE_NONE:

            return EXIT_FAILURE;
    }

    // Render
    mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "miniRT", true);
    if (!mlx) {
        fprintf(stderr, "Error initializing MLX42: %s\n", mlx_strerror(mlx_errno));
        return(1);
    }
    image = mlx_new_image(mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!image || mlx_image_to_window(mlx, image, 0, 0) < 0)
        return(1);
    // mlx_close_hook(mlx, &esc_exit, NULL);
    mlx_loop_hook(mlx, render_pixel_row_hook, render);
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return(0);
}
