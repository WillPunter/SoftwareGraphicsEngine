#ifndef SYSTEM_EVENT_COMMON_H
#define SYSTEM_EVENT_COMMON_H

#define SYSTEM_EVENT_COUNT 32

/* At this (early) stage in development, system events will be
   added as needed. */

/* Forward declare window structure */
struct system_window_t;
typedef struct system_window_t system_window_t;

typedef enum  {
    SYSTEM_EVENT_NONE,
    SYSTEM_EVENT_EXIT
} system_event_code_t;

typedef void (*system_event_handler_t)(system_window_t *window, void *, void *);
typedef system_event_handler_t system_event_table_t[SYSTEM_EVENT_COUNT];

#endif