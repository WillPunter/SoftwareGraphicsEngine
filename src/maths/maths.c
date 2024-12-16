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