#include "maths.h"

/* Projection */
maths_vec2f maths_project_vertex_3f (double viewing_plane_distance, int buffer_width, int buffer_height, double view_width, double view_height, maths_vec3f v) {
    /* Project onto viewing plane in 3d space */
    maths_vec2f result;
    result.x = v.x * viewing_plane_distance / v.z;
    result.y = v.y * viewing_plane_distance / v.z;

    printf (">>> %lf %lf -> %lf %lf\n", v.x, v.y, result.x, result.y);

    /* Convert to pixel space */
    result.x = result.x * buffer_width / view_width;
    result.y = result.y * buffer_height / view_height;

    printf ("%lf %lf\n", result.x, result.y);

    result.x += buffer_width / 2.f;
    result.y += buffer_height / 2.f;

    printf ("%lf %lf\n", result.x, result.y);

    return result;
}

maths_vec2f maths_project_vertex_4f_3d (double viewing_plane_distance, int buffer_width, int buffer_height, double view_width, double view_height, maths_vec4f v) {
    /* Project onto viewing plane in 3d space */
    maths_vec2f result;
    result.x = v.x * viewing_plane_distance / v.z;
    result.y = v.y * viewing_plane_distance / v.z;

    /* Convert to pixel space */
    result.x = result.x * buffer_width / view_width;
    result.y = result.y * buffer_height / view_height;

    result.x += buffer_width / 2.f;
    result.y += buffer_height / 2.f;

    return result;
}

maths_mat4x4f maths_model_transform (maths_vec4f position, maths_vec4f scale, maths_vec4f rotation) {
    /* scale, then rotate, then transform */
    maths_mat4x4f scale_matrix = maths_mat4x4f_enlargment (scale.x, scale.y, scale.z, scale.w);
    maths_mat4x4f rotation_matrix = maths_4x4f_rotation_yxz_3d (rotation.x, rotation.y, rotation.z);
    maths_mat4x4f translation_matrix = maths_4x4f_translation_3d (position.x, position.y, position.z);
    return maths_mat4x4f_mul (translation_matrix, maths_mat4x4f_mul (rotation_matrix, scale_matrix));
};
