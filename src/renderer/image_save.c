#include "../lib/MLX42/include/MLX42/MLX42.h"
#include <unistd.h> // For usleep

// This #define should only be in one .c file that includes the header.
// It creates the implementation of the library.
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"

/*
** Saves the content of an mlx_image_t to a PNG file.
**
** @param image The MLX image to save.
** @param filename The path for the output file (e.g., "output.png").
** @return Returns 0 on failure, non-zero on success.
*/
int save_image_to_png(mlx_image_t* image, const char* filename)
{
    // MLX42 stores pixels in RGBA format (4 bytes per pixel).
    const int components = 4;
    int stride = image->width * components;

    printf("Attempting to save image to %s...\n", filename);

    // Use stbi_write_png to write the image data to a file.
    // image->pixels is the raw buffer of your rendered image.
    int success = stbi_write_png(filename, image->width, image->height, components, image->pixels, stride);

    if (success)
    {
        printf("✅ Image saved successfully.\n");
    }
    else
    {
        fprintf(stderr, "❌ Error saving image.\n");
    }
    return (success);
}