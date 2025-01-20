#ifndef HOOKS_H
#define HOOKS_H

#include "../lib/MLX42/include/MLX42/MLX42.h"


typedef struct s_hooks_data {
    mlx_t           *mlx;
    mlx_image_t     *image;
    char            *title;

} t_hooks_data;


#endif
