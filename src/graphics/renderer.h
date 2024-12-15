#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "./../system/window.h"
#include <stdint.h>

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t pad;
} graphics_pixel_t;

typedef struct {
    unsigned int width;
    unsigned int height;
    graphics_pixel_t *pixels;
} graphics_renderer_t;

graphics_renderer_t *graphics_renderer_init (unsigned int width, unsigned int height);
void graphics_renderer_display (graphics_renderer_t *renderer, system_window_t *window);

#endif