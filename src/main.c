#include "renderer/mlx_setup.h"
#include "renderer/render_launch.h"
#include "renderer/threads/thread.h"
#include "system/scene_setup.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct app_options_s {
    bool        preview;
    const char  *scene_arg;
} app_options_t;

typedef struct render_thread_args_s {
    render_context_t    *render;
    uint64_t            initial_seed;
} render_thread_args_t;

static const struct scene_entry_s {
    const char   *name;
    scene_id_t    id;
} g_scene_entries[] = {
    {"SCENE_BOUNCING_SPHERES", SCENE_BOUNCING_SPHERES},
    {"SCENE_CORNELL_BOX_EMPTY", SCENE_CORNELL_BOX_EMPTY},
    {"SCENE_CORNELL_BOX_STANDARD", SCENE_CORNELL_BOX_STANDARD},
    {"SCENE_TOUCHING_SPHERES", SCENE_TOUCHING_SPHERES},
    {"SCENE_QUAD", SCENE_QUAD},
    {"SCENE_TWO_CHECKERED_SPHERES", SCENE_TWO_CHECKERED_SPHERES},
    {"SCENE_CORNELL_FOG", SCENE_CORNELL_FOG},
    {"SCENE_SIMPLE_LIGHT", SCENE_SIMPLE_LIGHT},
    {"SCENE_PERLIN_SPHERES", SCENE_PERLIN_SPHERES},
    {"SCENE_EARTH", SCENE_EARTH},
    {"SCENE_FINAL", SCENE_FINAL},
    {"SCENE_FIVE_SPHERES", SCENE_FIVE_SPHERES},
};

static void render_in_background(void *param)
{
    render_thread_args_t *args = param;

    render_launch(args->render, args->initial_seed);
}

static void print_scene_list(void)
{
    puts("Available scenes:");
    for (size_t i = 0; i < sizeof(g_scene_entries) / sizeof(g_scene_entries[0]); ++i)
        printf("  %2zu: %s\n", i, g_scene_entries[i].name);
}

static void print_usage(const char *program_name)
{
    printf("Usage: %s [--preview|-p] <scene-id|scene-name>\n", program_name);
    print_scene_list();
}

static int parse_options(int argc, char **argv, app_options_t *options)
{
    memset(options, 0, sizeof(*options));
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "--preview") == 0 || strcmp(argv[i], "-p") == 0)
            options->preview = true;
        else if (!options->scene_arg)
            options->scene_arg = argv[i];
        else
            return 0;
    }
    return options->scene_arg != NULL;
}

static scene_id_t parse_scene_arg(const char *arg)
{
    if (!arg)
        return SCENE_NONE;

    for (size_t i = 0; i < sizeof(g_scene_entries) / sizeof(g_scene_entries[0]); ++i) {
        if (strcmp(arg, g_scene_entries[i].name) == 0)
            return g_scene_entries[i].id;
    }

    char *endptr = NULL;
    long value = strtol(arg, &endptr, 10);
    if (*arg != '\0' && *endptr == '\0') {
        for (size_t i = 0; i < sizeof(g_scene_entries) / sizeof(g_scene_entries[0]); ++i) {
            if (value == (long)g_scene_entries[i].id)
                return g_scene_entries[i].id;
        }
    }

    return SCENE_NONE;
}

int main(int argc, char **argv)
{
    app_options_t options;

    if (!parse_options(argc, argv, &options)) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    scene_id_t scene_id = parse_scene_arg(options.scene_arg);
    if (scene_id == SCENE_NONE) {
        fprintf(stderr, "Invalid scene argument: %s\n", options.scene_arg);
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    uint64_t            initial_seed = (uint64_t)time(NULL);
    rt_random_seed(initial_seed);

    render_context_t    *render = calloc(1, sizeof(render_context_t));
    if (!render)
        return 1;

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
    render->preview_chunks = options.preview;

    if (options.preview) {
        render_thread_args_t thread_args = {
            .render = render,
            .initial_seed = initial_seed,
        };
        my_thread_t *render_thread;

        printf("Preview mode enabled: chunks will appear as they finish.\n");
        render_thread = thread_create(render_in_background, &thread_args);
        if (!render_thread)
        {
            mlx_terminate(mlx);
            return 1;
        }
        mlx_loop(mlx);
        thread_join(render_thread);
    } else {
        render_launch(render, initial_seed);
        mlx_loop(mlx);
    }
    mlx_terminate(mlx);
    return 0;
}
