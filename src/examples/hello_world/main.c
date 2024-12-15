#include "../../system/window.h"
#include "../../graphics/renderer.h"
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

    while (is_running) {
        intensity = (intensity + 1) % 255;

        renderer->pixels[640 * y + x].red += 10;
        renderer->pixels[640 * y + x].green += 30;
        renderer->pixels[640 * y + x].blue += 50; 

        x = x + 1;

        if (x >= 640) {
            x = 0;
            y = (y + 1) % 480;
        }

        graphics_renderer_display (renderer, window); 

        system_window_handle_events (window);
    }

    system_window_destroy (window);

    system_window_cleanup ();

    return 0;
}