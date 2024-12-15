#include "renderer.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

graphics_renderer_t *graphics_renderer_init (unsigned int width, unsigned int height) {
    graphics_renderer_t *renderer = (graphics_renderer_t *) malloc (sizeof (graphics_renderer_t));

    if (renderer == NULL) {
        fprintf (stderr, "Error - graphics/renderer: could not allocate memory for renderer.\n");
        return NULL;
    }

    renderer->pixels = (graphics_pixel_t *) malloc (sizeof (graphics_pixel_t) * width * height);

    if (renderer->pixels == NULL) {
        fprintf (stderr, "Error - graphics/renderer: could not allocate memory for render buffer.\n");
        free (renderer);
        return NULL;
    }

    renderer->width = width;
    renderer->height = height;

    return renderer;    
};

void graphics_renderer_display (graphics_renderer_t *renderer, system_window_t *window) {
    system_window_render_buffer_to_screen (window, renderer->pixels);
};

void graphics_renderer_clear_buffer (graphics_renderer_t *renderer) {
    memset (renderer->pixels, 0, sizeof (graphics_pixel_t) * renderer->width * renderer->height);
};

void graphics_renderer_draw_pixel (graphics_renderer_t *renderer, int x, int y, uint8_t red, uint8_t green, uint8_t blue) {
    graphics_pixel_t *px = &renderer->pixels[y * renderer->width + x];
    px->red = red;
    px->green = green;
    px->blue = blue;
};
