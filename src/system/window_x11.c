#include "window_x11.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Window structure implementation */
struct system_window_t  {
    int screen;       /* Handle for a screen to render to. */
    Window window;    /* Handle to the window to render to. */
};

/* Handle for a connection to the X server
   - effectively a socket. */
static Display *x_server_connection;

/* Initialise the window module. */
bool system_window_init () {
    /* Open connection to the X server. */
    x_server_connection = XOpenDisplay (NULL);

    if (x_server_connection == NULL) {
        fprintf (stderr, "Error - system/window: could not open X display.\n");
        return false;
    }

    return true;
}

/* Create a winow */
system_window_t *system_window_create (
                           const char *name,
                           unsigned int width,
                           unsigned int height,
                           bool show) {

    assert (name != NULL);

    /* Allocate window structure */
    system_window_t *window = (system_window_t *) malloc (sizeof (system_window_t));

    if (window == NULL) {
        fprintf (
            stderr,
            "Error - system/window: could not allocate"
            "space for window structure during creation."
        );

        free (window);
        return NULL;
    }

    /* Get the default screen to display the
       window to. */
    window->screen = DefaultScreen (x_server_connection);

    /* Create the window using X11 */
    window->window = XCreateSimpleWindow (
        x_server_connection,
        RootWindow (x_server_connection, window->screen),
        SYSTEM_WINDOW_DEFAULT_X,
        SYSTEM_WINDOW_DEFAULT_Y,
        width,
        height,
        1,
        BlackPixel (x_server_connection, window->screen),
        WhitePixel (x_server_connection, window->screen)
    );

    if (window->window == 0) {
        fprintf (stderr, "Error - system/window: could not create window\n");
        free (window);
        return NULL;
    }

    /* Set window name / title */
    XStoreName (x_server_connection, window->window, name);

    /* Flush X serve commands from this connection.
       The X server handles all currently open windows
       for the OS, so it is important that its queue
       is flushed for this connection specifically in
       order to see results. */
    XFlush (x_server_connection);
 
    return window;
}

void system_window_destroy (system_window_t *window) {
    assert (window != NULL);

    XDestroyWindow (x_server_connection, window->window);
    free (window);
}

void system_window_cleanup () {
    XCloseDisplay (x_server_connection);
}

void system_window_set_shown (system_window_t *window, bool shown) {
    assert (window != NULL);
    
    if (shown) {
        XMapWindow (x_server_connection, window->window);
    } else {
        XUnmapWindow (x_server_connection, window->window);
    }

    XFlush (x_server_connection);
}