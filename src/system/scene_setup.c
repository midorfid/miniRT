#include "system/scene_setup.h"

void    setup_scene(render_context_t *render, scene_id_t scene_id)
{
    switch (scene_id)
    {
        case SCENE_BOUNCING_SPHERES:
            render->image.aspect_ratio       = 16.0 / 9.0;
            render->camera.samples_per_pixel = 100;
            render->image.image_width        = 400;
            render->camera.max_depth         = 50;
            render->camera.lookfrom          = point3(13, 2, 3);
            render->camera.lookat            = point3(0, 0, 0);
            render->camera.vup               = vec3(0, 1, 0);
            render->camera.lens.defocus_angle = 0.6;
            render->camera.lens.focus_dist   = 10.0;
            render->pov.vfov                 = 20;
            render->world                    = bouncing_spheres();
            render->render_mode              = RENDER_MODE_AMBIENT;
            break;

        case SCENE_CORNELL_BOX_EMPTY:
            render->image.aspect_ratio       = 1.0;
            render->image.image_width        = 600;
            render->camera.samples_per_pixel = 50;
            render->camera.max_depth         = 50;
            render->pov.vfov                 = 40;
            render->camera.lookfrom          = point3(278, 278, -800);
            render->camera.lookat            = point3(278, 278, 0);
            render->camera.vup               = vec3(0, 1, 0);
            render->camera.lens.defocus_angle = 0;
            render->world                    = cornell_box_empty(&render->lights);
            render->render_mode              = RENDER_MODE_LIT;
            break;

        case SCENE_CORNELL_BOX_STANDARD:
            render->image.aspect_ratio       = 1.0;
            render->image.image_width        = 600;
            render->camera.samples_per_pixel = 200;
            render->camera.max_depth         = 50;
            render->pov.vfov                 = 40;
            render->camera.lookfrom          = point3(278, 278, -800);
            render->camera.lookat            = point3(278, 278, 0);
            render->camera.vup               = vec3(0, 1, 0);
            render->camera.lens.defocus_angle = 0;
            render->world                    = cornell_box_standard(&render->lights);
            render->render_mode              = RENDER_MODE_LIT;
            break;

        case SCENE_TOUCHING_SPHERES:
            render->image.aspect_ratio       = 1.0;
            render->image.image_width        = 600;
            render->camera.samples_per_pixel = 20;
            render->camera.max_depth         = 50;
            render->pov.vfov                 = 40;
            render->camera.lookfrom          = point3(0, 0, 0.1);
            render->camera.lookat            = point3(0, 0, -1);
            render->camera.vup               = vec3(0, 1, 0);
            render->world                    = touching_spheres();
            render->render_mode              = RENDER_MODE_AMBIENT;
            break;

        case SCENE_FIVE_SPHERES:
            render->image.aspect_ratio       = 16.0 / 9.0;
            render->image.image_width        = 400;
            render->camera.samples_per_pixel = 10;
            render->camera.max_depth         = 50;
            render->pov.vfov                 = 90;
            render->camera.lookfrom          = point3(-2, 2, 1);
            render->camera.lookat            = point3(0, 0, -1);
            render->camera.vup               = vec3(0, 1, 0);
            render->world                    = five_spheres();
            render->render_mode              = RENDER_MODE_AMBIENT;
            break;

        case SCENE_QUAD:
            render->image.aspect_ratio       = 1.0;
            render->image.image_width        = 400;
            render->camera.samples_per_pixel = 10;
            render->camera.max_depth         = 50;
            render->pov.vfov                 = 80;
            render->camera.lookfrom          = point3(0, 0, 9);
            render->camera.lookat            = point3(0, 0, 0);
            render->camera.vup               = vec3(0, 1, 0);
            render->world                    = quads(&render->lights);
            render->render_mode              = RENDER_MODE_LIT;
            break;

        case SCENE_TWO_CHECKERED_SPHERES:
            render->image.aspect_ratio       = 16.0 / 9.0;
            render->image.image_width        = 400;
            render->image.image_height       = 400;
            render->camera.samples_per_pixel = 10;
            render->camera.max_depth         = 50;
            render->pov.vfov                 = 20;
            render->camera.lookfrom          = point3(13, 2, 3);
            render->camera.lookat            = point3(0, 0, 0);
            render->camera.vup               = vec3(0, 1, 0);
            render->camera.lens.defocus_angle = 0;
            render->world                    = checkered_spheres();
            render->render_mode              = RENDER_MODE_AMBIENT;
            break;

        case SCENE_PERLIN_SPHERES:
            render->image.aspect_ratio       = 16.0 / 9.0;
            render->image.image_width        = 400;
            render->camera.samples_per_pixel = 100;
            render->camera.max_depth         = 50;
            render->pov.vfov                 = 20;
            render->camera.lookfrom          = point3(13, 2, 3);
            render->camera.lookat            = point3(0, 0, 0);
            render->camera.vup               = vec3(0, 1, 0);
            render->camera.lens.defocus_angle = 0;
            render->world                    = perlin_spheres();
            render->render_mode              = RENDER_MODE_AMBIENT;
            break;

        case SCENE_CORNELL_FOG:
            render->image.aspect_ratio       = 1.0;
            render->image.image_width        = 600;
            render->camera.samples_per_pixel = 200;
            render->camera.max_depth         = 50;
            render->pov.vfov                 = 40;
            render->camera.lookfrom          = point3(278, 278, -800);
            render->camera.lookat            = point3(278, 278, 0);
            render->camera.vup               = vec3(0, 1, 0);
            render->camera.lens.defocus_angle = 0;
            render->world                    = cornell_smoke(&render->lights);
            render->render_mode              = RENDER_MODE_LIT;
            break;

        case SCENE_SIMPLE_LIGHT:
            render->image.aspect_ratio       = 16.0 / 9.0;
            render->image.image_width        = 400;
            render->camera.samples_per_pixel = 10;
            render->camera.max_depth         = 50;
            render->pov.vfov                 = 20;
            render->camera.lookfrom          = point3(26, 3, 6);
            render->camera.lookat            = point3(0, 2, 0);
            render->camera.vup               = vec3(0, 1, 0);
            render->camera.lens.defocus_angle = 0;
            render->world                    = simple_light(&render->lights);
            render->render_mode              = RENDER_MODE_LIT;
            break;

        case SCENE_EARTH:
            render->image.aspect_ratio       = 16.0 / 9.0;
            render->image.image_width        = 400;
            render->camera.samples_per_pixel = 100;
            render->camera.max_depth         = 50;
            render->pov.vfov                 = 20;
            render->camera.lookfrom          = point3(0, 0, 12);
            render->camera.lookat            = point3(0, 0, 0);
            render->camera.vup               = vec3(0, 1, 0);
            render->camera.lens.defocus_angle = 0;
            render->world                    = earth();
            render->render_mode              = RENDER_MODE_AMBIENT;
            break;

        case SCENE_FINAL:
            render->image.aspect_ratio       = 1.0;
            render->image.image_width        = 400;
            render->camera.samples_per_pixel = 100;
            render->camera.max_depth         = 4;
            render->pov.vfov                 = 40;
            render->camera.lookfrom          = point3(478, 278, -600);
            render->camera.lookat            = point3(278, 278, 0);
            render->camera.vup               = vec3(0, 1, 0);
            render->camera.lens.defocus_angle = 0;
            render->world                    = final_scene(&render->lights);
            render->render_mode              = RENDER_MODE_LIT;
            break;

        case SCENE_NONE:
            break;
    }
}
