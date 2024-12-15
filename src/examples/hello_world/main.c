#include "../../system/window.h"
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

    assert (window != NULL);

    system_window_bind_event (window, SYSTEM_EVENT_EXIT, terminate);

    system_window_set_shown (window, true);

    is_running = true;

    while (is_running) {
        system_window_handle_events (window);
    }

    system_window_destroy (window);

    system_window_cleanup ();

    return 0;
}