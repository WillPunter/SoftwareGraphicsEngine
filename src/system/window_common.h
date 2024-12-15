#ifndef SYSTEM_WINDOW_COMMON_H
#define SYSTEM_WINDOW_COMMON_H

#include <stdbool.h>
#include <assert.h>
#include "event_common.h"

/* Window structure */
struct system_window_t;
typedef struct system_window_t system_window_t;

/* API functions */
bool system_window_init ();
system_window_t *system_window_create (const char *name, unsigned int width, unsigned int height, bool show);
void system_window_destroy (system_window_t *window);
void system_window_cleanup ();
void system_window_set_shown (system_window_t *window, bool shown);
bool system_window_bind_event_table (system_window_t *window, system_event_table_t et);
bool system_window_bind_event (system_window_t *window, system_event_code_t event, system_event_handler_t callback);
void system_window_handle_events (system_window_t *window);

#endif