#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include "./../system/window.h"
#include <stdint.h>

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t pad;
} graphics_pixel_t;

typedef struct {
    unsigned int width;
    unsigned int height;
    graphics_pixel_t *pixels;
} graphics_renderer_t;

graphics_renderer_t *graphics_renderer_init (unsigned int width, unsigned int height);
void graphics_renderer_display (graphics_renderer_t *renderer, system_window_t *window);
void graphics_renderer_clear_buffer (graphics_renderer_t *renderer);
void graphics_renderer_draw_pixel (graphics_renderer_t *renderer, int x, int y, uint8_t red, uint8_t green, uint8_t blue);
void graphics_renderer_draw_line (graphics_renderer_t *renderer, int x0, int y0, int x1, int y1, uint8_t red, uint8_t green, uint8_t blue);
void graphics_renderer_draw_wireframe_triangle (graphics_renderer_t *renderer, int x0, int y0, int x1, int y1, int x2, int y2, uint8_t red, uint8_t green, uint8_t blue);
void graphics_renderer_draw_filled_triangle (graphics_renderer_t *renderer, int x0, int y0, int x1, int y1, int x2, int y2, uint8_t red, uint8_t green, uint8_t blue);
void graphics_renderer_draw_shaded_triangle (graphics_renderer_t *renderer, int x0, int y0, int x1, int y1, int x2, int y2, uint8_t r_0, uint8_t g_0, uint8_t b_0, uint8_t r_1, uint8_t g_1, uint8_t b_1, uint8_t r_2, uint8_t g_2, uint8_t b_2);

#endif