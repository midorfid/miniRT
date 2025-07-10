#include "../include/render_context.h"

color_t     ray_color(const ray_t *r, const hittable_list_t *world, int depth) {
    hit_record_t    rec;

    if (depth <= 0)
        return vec3(0.0, 0.0, 0.0);
    // printf("ray origin: %f, %f, %f\n", r->orig.x, r->orig.y, r->orig.z);
    // printf("ray dir: %f, %f, %f\n", r->dir.x, r->dir.y, r->dir.z);
    fflush(stdout);
    if (hittable_list_hit_test(r, world, 0.001, INFINITY, &rec)) {
        
        ray_t       scattered;
        color_t     attenuation;
        color_t     color_from_emmision = material_emmit(rec.mat, rec.u, rec.v, &rec.p);
        if (material_scatter(rec.mat, r, &rec, &attenuation, &scattered)) {
            color_t color_from_scatter = vec3_multi(ray_color(&scattered, world, depth-1), attenuation); 
            return vec3_sum(color_from_emmision, color_from_scatter);
        }
        // printf("color x: %f, y: %f, z: %f\n", color_from_emmision.x, color_from_emmision.y, color_from_emmision.z);
        return (color_from_emmision);
    }
    vec3_t  unit_direction = vec3_normalize(r->dir);
    double  a = 0.5 * (unit_direction.y + 1.0);
    return(vec3_sum(vec3_scaled_return(vec3(1.0,1.0,1.0), 1.0-a), vec3_scaled_return(vec3(0.5,0.7,1.0), a)));
}

render_context_t        *render_context_new(render_context_t *render_contxt, mlx_t *mlx, mlx_image_t *mlx_img) {
    render_contxt->mlx = mlx;
    render_contxt->mlx_image = mlx_img;
    render_context_init(render_contxt);
    return render_contxt;
}

static void        image_settings_init(image_t *image) {
    image->image_height = (int)(image->image_width / image->aspect_ratio);
    image->image_height = (image->image_height < 1) ? 1 : image->image_height;

}

static void        lens_init(defocus_blur_t *lens, vec3_t u, vec3_t v) {
    lens->defocus_angle = 0; // Variation angle of rays through each pixel

    // Calculate camera defocus disk basis vectors
    double      defocus_radius = lens->focus_dist * tan(DEG_TO_RAD(lens->defocus_angle / 2));
    lens->defocus_disk_u = vec3_scaled_return(u, defocus_radius); // defocus disk horizontal radius
    lens->defocus_disk_v = vec3_scaled_return(v, defocus_radius); // defocus disk vertical radius
}

static void        camera_settings_init(camera_t *camera) {
    camera->sqrt_spp = sqrt(camera->samples_per_pixel);
    camera->pixel_sample_scale = 1.0 / (camera->sqrt_spp * camera->sqrt_spp);
    camera->rec1p_sqrt_spp = 1.0 / camera->sqrt_spp; 
    // color_t             background = color_in(0.70, 0.80, 1.00); // TODO
    camera->lens.focus_dist = vec3_len(vec3_sub_return(camera->lookfrom, camera->lookat)); // Distance from camera lookfrom point to plane of perfect focus

    camera->camera_center = camera->lookfrom;

    camera->w = vec3_normalize(vec3_sub_return(camera->lookfrom, camera->lookat));
    camera->u = vec3_normalize(vec3_cross(camera->vup, camera->w));
    camera->v = vec3_cross(camera->w, camera->u);

    lens_init(&camera->lens, camera->u, camera->v);
}

static void        viewpoint_init(viewpoint_t *view_p, double focus_dist, double width_to_height_ratio, vec3_t u, vec3_t negative_v) {
    // Determine viewpoint dimensions
    view_p->theta = DEG_TO_RAD(view_p->vfov);
    view_p->h = tan(view_p->theta/2);
    view_p->viewport_height = 2 * view_p->h * focus_dist;
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
    image_settings_init(&render->image);
    camera_settings_init(&render->camera);
    
    double image_w_h_ratio = (double)render->image.image_width / render->image.image_height;
    viewpoint_init(&render->pov, render->camera.lens.focus_dist, image_w_h_ratio, render->camera.u, vec3_negative(&render->camera.v));

    vec3_t      viewport_upper_left_pixel = 
        vec3_sub_return(vec3_sub_return(
            vec3_sub_return(render->camera.camera_center, vec3_scaled_return(render->camera.w, render->camera.lens.focus_dist)), 
            vec3_scaled_return(render->pov.viewport_u, 0.5)), vec3_scaled_return(render->pov.viewport_v, 0.5));
    render_plane_init(&render->render_p, viewport_upper_left_pixel, &render->image, render->pov.viewport_u, render->pov.viewport_v);
}
