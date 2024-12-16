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

static void swap_int (int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

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
            swap_int (&x0, &x1);
            swap_int (&y0, &y1);
        }

        interpolate (x0, y0, x1, y1, draw_line_pixel_dx, (void*) renderer, (void*) &px);
    } else {
        /* Line is closer to vertical than horizontal.
        Interpolate with respect to dy. */
        if (y0 > y1) {
            swap_int (&x0, &x1);
            swap_int (&y0, &y1);
        }

        interpolate (y0, x0, y1, x1, draw_line_pixel_dy, (void*) renderer, (void*) &px);
    }
}

void graphics_renderer_draw_wireframe_triangle (graphics_renderer_t *renderer, int x0, int y0, int x1, int y1, int x2, int y2, uint8_t red, uint8_t green, uint8_t blue) {
    graphics_renderer_draw_line (renderer, x0, y0, x1, y1, red, green, blue);
    graphics_renderer_draw_line (renderer, x1, y1, x2, y2, red, green, blue);
    graphics_renderer_draw_line (renderer, x2, y2, x0, y0, red, green, blue);   
};

static void draw_horizontal_line (graphics_renderer_t *renderer, int x0, int x1, int y, uint8_t red, uint8_t green, uint8_t blue) {
    if (x0 > x1) {
        swap_int (&x0, &x1);
    }

    for (int i = x0; i <= x1; i++) {
        graphics_renderer_draw_pixel (renderer, i, y, red, green, blue);
    }
};

void graphics_renderer_draw_filled_triangle (graphics_renderer_t *renderer, int x0, int y0, int x1, int y1, int x2, int y2, uint8_t red, uint8_t green, uint8_t blue) {
    /* Sort the points in vertical order */
    if (y1 < y0) {
        swap_int (&x0, &x1);
        swap_int (&y0, &y1);
    }

    if (y2 < y0) {
        swap_int (&x0, &x2);
        swap_int (&y0, &y2);
    }

    if (y2 < y1) {
        swap_int (&x1, &x2);
        swap_int (&y1, &y2);
    }

    /* Compute change in x per unit y for each line 
       segment. */
    int dy_0_1 = y1 - y0;
    int dy_0_2 = y2 - y0;
    int dy_1_2 = y2 - y1;

    /* Render first half of triangle: (x0, y0) -> (x1, y1) and (x', y1)*/
    double p1 = x0;
    double p2 = x0;
        
    if (y1 > y0) {
        double x_0_1 = (x1 - x0) / (double) dy_0_1;
        double x_0_2 = (x2 - x0) / (double) dy_0_2;
    
        for (int i = y0; i <= y1; i++) {
            draw_horizontal_line (renderer, (int) p1, (int) p2, i, red, green, blue);
            p1 += x_0_1;
            p2 += x_0_2;
        }
    }

    /* Draw second half of triangle (x1, y1) and (x', y1) -> (x2, y2) */
    p1 = x1;

    if (y2 > y1) {
        double x_1_2 = (x2 - x1) / (double) dy_1_2;
        double x_0_2 = (x2 - x0) / (double) dy_0_2;

        for (int i = y1; i <= y2; i++) {
            draw_horizontal_line (renderer, (int) p1, (int) p2, i, red, green, blue);
            p1 += x_1_2;
            p2 += x_0_2;
        }
    } else {
        draw_horizontal_line (renderer, (int) p1, (int) p2, y1, red, green, blue);
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
