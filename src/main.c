#include "renderer/mlx_setup.h"
#include "renderer/render_launch.h"
#include "system/scene_setup.h"
#include <stdlib.h>
#include <time.h>

int main(void)
{
    uint64_t            initial_seed = (uint64_t)time(NULL);
    rt_random_seed(initial_seed);

    render_context_t    *render = calloc(1, sizeof(render_context_t));
    if (!render)
        return 1;

    scene_id_t scene_id = SCENE_FINAL;
    if (scene_id == SCENE_NONE)
        return EXIT_FAILURE;
    setup_scene(render, scene_id);

    int img_w = render->image.image_width;
    int img_h = render->image.image_height > 0
        ? render->image.image_height
        : (int)(img_w / render->image.aspect_ratio);
    img_h = img_h < 1 ? 1 : img_h;

    mlx_t               *mlx;
    mlx_image_t         *image;
    if (!setup_mlx(&mlx, &image, img_w, img_h, "miniRT"))
        return 1;

    render = render_context_new(render, mlx, image);

    render_launch(render, initial_seed);
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return 0;
}
