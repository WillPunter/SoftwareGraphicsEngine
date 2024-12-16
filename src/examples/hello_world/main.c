#include "../../system/window.h"
#include "../../graphics/renderer.h"
#include "../../maths/maths.h"
#include "../../resources/resources.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

bool is_running;

void terminate (system_window_t *window, void *aux_1, void *aux_2) {
    printf ("Terminating...\nGoodbye!\n");
    is_running = false;
}

int main () {
    system_window_init ();

    system_window_t *window = system_window_create ("Hello World!!!", 640, 480, true);
    graphics_renderer_t *renderer = graphics_renderer_init (640, 480);

    assert (window != NULL);

    system_window_bind_event (window, SYSTEM_EVENT_EXIT, terminate);

    system_window_set_shown (window, true);

    resources_mesh_t *mesh = resources_load_mesh_from_obj_file ("./build/res/cube.obj");
    maths_vec3f cube_pos;
    cube_pos.x = -5;
    cube_pos.y = 1;
    cube_pos.z = 15;

    is_running = true;

    while (is_running) {
        graphics_renderer_clear_buffer (renderer);

        for (int i = 0; i < mesh->num_faces; i++) {
            maths_vec3f v[3];
            
            for (int j = 0; j < 3; j++) {
                //printf ("faces: %d %d %d\n", mesh->faces[i][0], mesh->faces[i][1], mesh->faces[i][j]);
                v[j] = mesh->vertices[mesh->faces[i][j]].coord;
                v[j].x += cube_pos.x;
                v[j].y += cube_pos.y;
                v[j].z += cube_pos.z;
            }

            printf ("%f %f %f\n", v[0].x, v[1].x, v[2].x);

            /* Project vertices */
            maths_vec2f p[3];

            for (int j = 0; j < 3; j++) {
                p[j] = maths_project_vertex_3f (1, 640, 480, 2, 2, v[j]);
            }

            printf ("px %f %f %f\n", p[0].x, p[1].x, p[2].x);

            graphics_renderer_draw_wireframe_triangle (renderer, p[0].x, p[0].y, p[1].x, p[1].y, p[2].x, p[2].y, 0, 0, 255 );
        }

        graphics_renderer_display (renderer, window); 

        system_window_handle_events (window);
    }

    system_window_destroy (window);

    system_window_cleanup ();

    return 0;
}