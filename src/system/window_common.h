#ifndef SYSTEM_WINDOW_COMMON_H
#define SYSTEM_WINDOW_COMMON_H

#include <stdbool.h>
#include <assert.h>

/* Window structure */
struct system_window_t;
typedef struct system_window_t system_window_t;

/* API functions */
bool system_window_init ();
system_window_t *system_window_create (const char *name, unsigned int width, unsigned int height, bool show);
void system_window_destroy (system_window_t *window);
void system_window_cleanup ();
void system_window_set_shown (system_window_t *window, bool shown);

#endif