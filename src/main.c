#include "../lib/MLX42/include/MLX42/MLX42.h"
#include <stdlib.h>
#include <stdio.h>


#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000


// void    esc_exit(mlx_t *mlx) {
//     mlx_terminate(mlx);
//     exit(0);
// }

int get_argb(int a, int r, int g, int b) {
    return (a << 24 | r << 16 | g << 8 | b);
}

int main(void) {

    // MLX42

    mlx_t           *mlx;
    mlx_image_t     *image;

    // Image

    int image_width = 256;
    int image_height = 256;

    // Render
    mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "miniRT", true);
    if (!mlx)
        return(1);
    image = mlx_new_image(mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!image || mlx_image_to_window(mlx, image, 0, 0) < 0)
        return(1);
    // mlx_close_hook(mlx, &esc_exit, NULL);
    for (int j = 0;j<image_height;++j){
        for (int i = 0;i<image_width;++i) {
            double r = (double)i / (image_width-1);
            double g = (double)j / (image_height-1);
            double b = 0.0;

            int ir = (int)(255.999 * r);
            int ig = (int)(255.999 * g);
            int ib = (int)(255.999 * b);

            // printf("r:%d g:%d b:%d", ir, ig, ib);
            int color = (ib << 8) | (ig << 16) | (ir << 24)| (0xFF);
            // printf("color:0x%08X\n", color);
             mlx_put_pixel(image, i, j, color);
        }
    }
    mlx_loop(mlx);
    mlx_terminate(mlx);
    return(0);
}
