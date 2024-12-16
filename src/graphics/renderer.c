#include "renderer.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void interpolate (double i0, double d0, double i1, double d1, void *(*func)(double, double, void *), void *aux);

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

/* Iterpolate over the independent variable, adjusting
   the dependent variable accordingly. */
static void interpolate (double i0, double d0, double i1, double d1, void *(*func)(double, double, void *), void *aux) {
    /* If there is no change in i, then
       simply call the function for i0, d0. */
    if (i0 == i1) {
        func (i0, d0, aux);
        return;
    }
    
    double gradient = (d1 - d0) / (i1 - i0);
    double d = d0;

    for (double i = i0; i <= i1; i++) {
        func (i, d, aux);
        d += gradient;
    }
};
