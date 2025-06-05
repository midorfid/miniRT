#include "../include/render_context.h"

color_t     ray_color(const ray_t *r, const hittable_list_t *world, int depth) {
    hit_record_t    rec;

    if (depth <= 0)
        return vec3(0.0, 0.0, 0.0);
    if (hittable_list_hit_test(r, world, 0.001, INFINITY, &rec)) {
        ray_t       scattered;
        color_t     attenuation;
        color_t     color_from_emmision = material_emmit(rec.mat, rec.u, rec.v, &rec.p);
        if (material_scatter(rec.mat, r, &rec, &attenuation, &scattered)) {
            color_t color_from_scatter = vec3_multi(ray_color(&scattered, world, depth-1), attenuation); 
            return vec3_sum(color_from_emmision, color_from_scatter);
        }
        return (color_from_emmision);
    }
    vec3_t  unit_direction = vec3_normalize(r->dir);
    double  a = 0.5 * (unit_direction.y + 1.0);
    return(vec3_sum(vec3_scaled_return(vec3(1.0,1.0,1.0), 1.0-a), vec3_scaled_return(vec3(0.5,0.7,1.0), a)));
}

render_context_t        *render_context_new(mlx_t *mlx, mlx_image_t *mlx_img) {
    render_context_t *render_contxt = calloc(1, sizeof(render_context_t));
    if (render_contxt == NULL) {
        printf("render context new failed");
        return NULL;
    }
    render_context_init(render_contxt);
    return render_contxt;
}

static void        image_settings_init(image_t *image) {
    image->aspect_ratio = 16.0 / 9.0;
    image->image_width = 400;
    image->image_height = (int)(image_width/aspect_ratio);
    image->image_height = (image_height < 1) ? 1 : image_height;

}

static void        lens_init(defocus_blur_t *lens, vec3_t u, vec3_t v) {
    lens->defocus_angle = 0.6; // Variation angle of rays through each pixel
    lens->focus_dist = 10.0; // Distance from camera lookfrom point to plane of perfect focus

    // Calculate camera defocus disk basis vectors
    double      defocus_radius = focus_dist * tan(DEG_TO_RAD(lens->defocus_angle / 2));
    lens->defocus_disk_u = vec3_scaled_return(u, defocus_radius); // defocus disk horizontal radius
    lens->defocus_disk_v = vec3_scaled_return(v, defocus_radius); // defocus disk vertical radius
}

static void        camera_settings_init(camera_t *camera) {
    camera->samples_per_pixel = 10;
    camera->pixel_sample_scale = 1.0 / camera->samples_per_pixel; 
    camera->max_depth = 50;
    camera->lookfrom = point3(13,2,3);
    camera->at = point3(0,0,0);
    camera->vup = vec3(0,1,0);
    // color_t             background = color_in(0.70, 0.80, 1.00); // TODO

    point3_t            camera_center = camera->lookfrom;

    camera->w = vec3_normalize(vec3_sub_return(camera->lookfrom, camera->lookat));
    camera->u = vec3_cross(camera->vup, camera->w);
    camera->v = vec3_cross(camera->w, camera->u);

    lens_init(&camera->lens, camera->u, camera->v);
}

static void        viewpoint_init(viewpoint_t *view_p, double focus_dist, double width_to_height_ratio, vec3_t u, vec3_t negative_v) {
    // Determine viewpoint dimensions
    view_p->vfov = 20;
    view_p->theta = DEG_TO_RAD(view_p->vfov);
    view_p->h = tan(view_p->theta/2);
    view_p->viewport_height = 2 * h * focus_dist;
    view_p->viewport_width = view_p->viewport_height * width_to_height_ratio;

    // Vectors across horizontal and down the vertical viewport edges
    view_p->viewport_u = vec3_scaled_return(u, view_p->viewport_width);
    view_p->viewport_v = vec3_scaled_return(negative_v, view_p->viewport_height);
}

static void        render_plane_init(render_plane_t *render_p, vec3_t upper_left_pixel, image_t *image, vec3_t viewport_u, vec3_t viewport_v) {
    // Pixel delta vectors
    render_p->pixel_delta_u = vec3_scaled_return(viewport_u, (1.0/image->image_width));
    render_p->pixel_delta_v = vec3_scaled_return(viewport_v, (1.0/image->image_height));

    // Location of upper left pixel
    render_p->pixel00_loc = vec3_sum(upper_left_pixel, 
        vec3_scaled_return(vec3_sum(render_p->pixel_delta_u, render_p->pixel_delta_v), 0.5));
}

static void        render_context_init(render_context_t *render)
{
    render->current_i = 0;
    render->current_j = 0;
    render->render_complete = false;

    image_settings_init(&render->image);
    camera_settings_init(&render->camera);
    
    double image_w_h_ratio = (double)(render->image.image_width / render->image.image_height);
    viewpoint_init(&render->pov, render->camera.lens.focus_dist, image_w_h_ratio, render->camera.u, vec3_negative(render->camera.v));

    vec3_t      viewport_upper_left_pixel = 
        vec3_sub_return(vec3_sub_return(
            vec3_sub_return(render->camera.camera_center, vec3_scaled_return(render->camera.w, render->camera.lens.focus_dist)), 
            vec3_scaled_return(render->pov.viewport_u, 0.5)), vec3_scaled_return(render->pov.viewport_v, 0.5));
    render_plane_init(&render->render_p, viewport_upper_left_pixel);
}
