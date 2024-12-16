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
    renderer->view_distance = 1;
    renderer->view_width = 2;
    renderer->view_height = 2;

    assert (window != NULL);

    system_window_bind_event (window, SYSTEM_EVENT_EXIT, terminate);

    system_window_set_shown (window, true);

    resources_mesh_t *mesh = resources_load_mesh_from_obj_file ("./build/res/cube.obj");

    resources_model_t cube_model;
    cube_model.mesh = mesh;
    cube_model.position = (maths_vec4f) { -5.0, 1.0, 15.0, 1.0 };
    cube_model.scale = (maths_vec4f) { 1.0, 1.0, 1.0, 1.0 };
    cube_model.rotation = (maths_vec4f) { 0.0, 0.0, 0.0, 0.0 };

    graphics_camera_t camera;
    camera.position = (maths_vec4f) { 0.0, 0.0, 0.0, 1.0 };
    camera.scale = (maths_vec4f) { 1.0, 1.0, 1.0, 1.0 };
    camera.rotation = (maths_vec4f) { 0.0, 0.0, 0.0 };

    is_running = true;

    while (is_running) {
        graphics_renderer_clear_buffer (renderer);

        //cube_model.rotation.x += 0.001;
        cube_model.rotation.x += 0.0005;
        camera.rotation.y += 0.01;
        graphics_renderer_render_model (renderer, &cube_model, &camera);

        graphics_renderer_display (renderer, window); 

        system_window_handle_events (window);
    }

    system_window_destroy (window);

    system_window_cleanup ();

    return 0;
}