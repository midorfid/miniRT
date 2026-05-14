#include "renderer/mlx_setup.h"
#include <stdio.h>

static void keep_alive_hook(void *param) { (void)param; }

static void key_hook(mlx_key_data_t keydata, void *param)
{
    mlx_t *mlx = param;

    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
        mlx_close_window(mlx);
}

int     setup_mlx(mlx_t **mlx, mlx_image_t **image, int width, int height, const char *title)
{
    *mlx = mlx_init(width, height, title, true);
    if (!*mlx)
    {
        fprintf(stderr, "Error initializing MLX42: %s\n", mlx_strerror(mlx_errno));
        return 0;
    }
    *image = mlx_new_image(*mlx, width, height);
    if (!*image || mlx_image_to_window(*mlx, *image, 0, 0) < 0)
        return 0;
    mlx_loop_hook(*mlx, keep_alive_hook, NULL);
    mlx_key_hook(*mlx, key_hook, *mlx);
    return 1;
}
