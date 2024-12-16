#include "renderer.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void interpolate (double i0, double d0, double i1, double d1, void (*func)(double, double, void *, void *), void *aux1, void *aux2);
 
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

static void draw_line_pixel_dx (double x, double y, void *aux1, void *aux2) {
    int x_i = (int) x;
    int y_i = (int) y;
    graphics_renderer_t *renderer = (graphics_renderer_t *) aux1;
    graphics_pixel_t *pixel = (graphics_pixel_t *) aux2;
    graphics_renderer_draw_pixel (renderer, x_i, y_i, pixel->red, pixel->green, pixel->blue);
};

static void draw_line_pixel_dy (double x, double y, void *aux1, void *aux2) {
    int x_i = (int) y;
    int y_i = (int) x;
    graphics_renderer_t *renderer = (graphics_renderer_t *) aux1;
    graphics_pixel_t *pixel = (graphics_pixel_t *) aux2;
    graphics_renderer_draw_pixel (renderer, x_i, y_i, pixel->red, pixel->green, pixel->blue);
};

void graphics_renderer_draw_line (graphics_renderer_t *renderer, int x0, int y0, int x1, int y1, uint8_t red, uint8_t green, uint8_t blue) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    graphics_pixel_t px;
    px.red = red;
    px.green = green;
    px.blue = blue;

    if (abs (dx) > abs (dy)) {
        /* Line is closer to horizontal than vertical.
           Interpolate with respect to dx from left to
           right. */
        if (x0 > x1) {
            int temp = x0;
            x0 = x1;
            x1 = temp;

            temp = y0;
            y0 = y1;
            y1 = temp;
        }

        interpolate (x0, y0, x1, y1, draw_line_pixel_dx, (void*) renderer, (void*) &px);
    } else {
        /* Line is closer to vertical than horizontal.
        Interpolate with respect to dy. */
        if (y0 > y1) {
            int temp = x0;
            x0 = x1;
            x1 = temp;

            temp = y0;
            y0 = y1;
            y1 = temp;
        }

        interpolate (y0, x0, y1, x1, draw_line_pixel_dy, (void*) renderer, (void*) &px);
    }
}


/* Iterpolate over the independent variable, adjusting
   the dependent variable accordingly. */
static void interpolate (double i0, double d0, double i1, double d1, void (*func)(double, double, void *, void *), void *aux1, void *aux2) {
    /* If there is no change in i, then
       simply call the function for i0, d0. */
    if (i0 == i1) {
        func (i0, d0, aux1, aux2);
        return;
    }
    
    double gradient = (d1 - d0) / (i1 - i0);
    double d = d0;

    for (double i = i0; i <= i1; i++) {
        func (i, d, aux1, aux2);
        d += gradient;
    }
};
