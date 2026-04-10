#include "shading/textures/texture_shared.h"

#define STB_IMAGE_IMPLEMENTATION
#include <renderer/stb_image.h>

typedef struct image_texture_s {
    texture_t  base;

    unsigned char   *image_data;

    int             width, height;
    int             base_per_scanline;
} image_texture_t;

static color_t      image_tex_getValue(const texture_t *texture, double u, double v, const point3_t *p);
static void         image_tex_delete(texture_t *texture);

texture_t    *image_tex_new(const char *filename) {
    image_texture_t     *image_tex = calloc(1, sizeof(image_texture_t));
    if (image_tex == NULL) {
        printf("image_tex calloc failed\n");
        return NULL;
    }
    int actual_channels = 0;
    int desired_channels = 3;

    image_tex->image_data = stbi_load(filename, &image_tex->width, &image_tex->height, &actual_channels, desired_channels);
    image_tex->base_per_scanline = image_tex->width * desired_channels;

    texture_t_innit(&image_tex->base, TEXTURE_TYPE_IMAGE_TEX, image_tex_delete, image_tex_getValue);
    return (texture_t *)image_tex;
}

static color_t      image_tex_getValue(const texture_t *texture, double u, double v, const point3_t *p) {
    image_texture_t *img_tex = (image_texture_t *)texture;
    
    if (img_tex->image_data == NULL)
        return color_in(0.0, 1.0, 1.0);

    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0);

    int i = (int)(u * img_tex->width);
    int j = (int)(v * img_tex->height);

    i = i >= img_tex->width ? img_tex->width - 1 : i;
    j = j >= img_tex->height ? img_tex->height - 1 : j;

    unsigned char *pixel = img_tex->image_data + j * img_tex->base_per_scanline + i * 3;
    
    return vec3_scaled_return(vec3(pixel[0], pixel[1], pixel[2]), 1.0 / 255.0);
}

static void         image_tex_delete(texture_t *texture) {
    image_texture_t *img_tex = (image_texture_t *)texture;
    if (img_tex->image_data != NULL)
        stbi_image_free(img_tex->image_data);
    free(img_tex);
}
