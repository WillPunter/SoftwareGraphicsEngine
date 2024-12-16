#include "../../system/window.h"
#include "../../graphics/renderer.h"
#include "../../maths/maths.h"
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

    is_running = true;

    uint8_t intensity = 0;

    uint32_t x = 0;
    uint32_t y = 0;
    double angle = 0;

    while (is_running) {
        graphics_renderer_clear_buffer (renderer);

        intensity = (intensity + 1) % 255;

        renderer->pixels[640 * y + x].red = 255;
        renderer->pixels[640 * y + x].green = 0;
        renderer->pixels[640 * y + x].blue = 0; 

        graphics_renderer_draw_shaded_triangle (renderer, 100, 200, 200, 100, 300, 300, 255, 0, 0, 0, 255, 0, 0, 0, 255);

        x = x + 1;

        if (x >= 640) {
            x = 0;
            y = (y + 1) % 480;
        }

        angle += 0.001;
        graphics_renderer_draw_line (renderer, 320, 240, 320 + 100 * cos (angle), 240 + 100 * sin (angle), 0, 255, 0);

        for (int i = 0; i < 100; i++) {
            renderer->pixels[640 * 400 + i].blue = 255;
        }

        graphics_renderer_display (renderer, window); 

        system_window_handle_events (window);
    }

    system_window_destroy (window);

    system_window_cleanup ();

    return 0;
}