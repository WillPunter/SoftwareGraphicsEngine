#include "../../system/window.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main () {
    system_window_init ();

    system_window_t *window = system_window_create ("Hello World!!!", 640, 480, true);

    assert (window != NULL);

    system_window_set_shown (window, true);

    sleep (10);

    system_window_destroy (window);

    system_window_cleanup ();

    return 0;
}