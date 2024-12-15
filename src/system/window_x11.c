#include "window_x11.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Window structure implementation */
struct system_window_t  {
    int screen;                       /* Handle for a screen to render to. */
    Window window;                    /* Handle to the window to render to. */
    system_event_table_t event_table; /* Event table */
};

/* Handle for a connection to the X server
   - effectively a socket. */
static Display *x_server_connection;

static system_event_code_t translate_event (XEvent *event);
static void dispatch_event (system_window_t *window, XEvent *event, system_event_code_t event_code);

static Atom wm_delete_window;

/* Initialise the window module. */
bool system_window_init () {
    /* Open connection to the X server. */
    x_server_connection = XOpenDisplay (NULL);

    if (x_server_connection == NULL) {
        fprintf (stderr, "Error - system/window: could not open X display.\n");
        return false;
    }

    /* Initialise close-window atom */
    wm_delete_window = XInternAtom (x_server_connection, "WM_DELETE_WINDOW", False);

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
    system_window_t *window = (system_window_t *) calloc (sizeof (system_window_t), 1);

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

    /* Set up events*/
    XSetWMProtocols (x_server_connection, window->window, &wm_delete_window, 1);
    XSelectInput (x_server_connection, window->window, ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask | ClientMessage);

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

bool system_window_bind_event_table (system_window_t *window, system_event_table_t et) {
    if (window == NULL) {
        fprintf (stderr, "Error - system/window: cannot bind event table to NULL window.");
        return false;
    }

    memcpy (window->event_table, et, sizeof (system_event_table_t));

    return true;
}

bool system_window_bind_event (system_window_t *window, system_event_code_t event, system_event_handler_t callback) {
    if (window == NULL) {
        fprintf (stderr, "Error - system/window: cannot bind event function to NULL window.");
        return false;
    } else if (callback == NULL) {
        fprintf (stderr, "Error - system/window: cannot bind NULL callback to window event.");
        return false;
    }
    
    window->event_table[event] = callback;

    return true;
};

void system_window_handle_events (system_window_t *window) {
    XEvent event;

    /* Iterate through all events in event queue. */
    while (XPending (x_server_connection)) {
        /* There is an event pending - obtain
           it's data and remove from queue. */
        XNextEvent (x_server_connection, &event);

        /* Translate Event */
        system_event_code_t evt = translate_event (&event);

        dispatch_event (window, &event, evt);
    }
}

static system_event_code_t translate_event (XEvent *event) {
    system_event_code_t evt = SYSTEM_EVENT_NONE;
    
    switch (event->type) {
        case ClientMessage: {
            if (event->xclient.data.l[0] == wm_delete_window) {
                evt = SYSTEM_EVENT_EXIT;
            }
            break;
        }
    }

    return evt;
}

static void dispatch_event (
    system_window_t *window,
    XEvent *event,
    system_event_code_t event_code
) {
    if (event_code != SYSTEM_EVENT_NONE && window->event_table[event_code] != NULL) {
        window->event_table[event_code](window, NULL, NULL);
    }
}
