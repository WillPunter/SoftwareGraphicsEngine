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

static void swap_uint_8 (uint8_t *x, uint8_t *y) {
    uint8_t temp = *x;
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

static void draw_horizontal_line_shaded (graphics_renderer_t *renderer, int x0, int x1, int y, uint8_t r_1, uint8_t g_1, uint8_t b_1, uint8_t r_2, uint8_t g_2, uint8_t b_2) {
    if (x0 > x1) {
        swap_int (&x0, &x1);
        swap_uint_8 (&r_1, &r_2);
        swap_uint_8 (&g_1, &g_2);
        swap_uint_8 (&b_1, &b_2);
    }

    double r = r_1;
    double g = g_1;
    double b = b_1;

    if (x1 > x0) {
        double dr = (r_2 - r_1) / (double) (x1 - x0);
        double dg = (g_2 - g_1) / (double) (x1 - x0);
        double db = (b_2 - b_1) / (double) (x1 - x0);
        
        for (int i = x0; i <= x1; i++) {
            graphics_renderer_draw_pixel (renderer, i, y, r, g, b);
            r += dr;
            g += dg;
            b += db;
        }
    } else {
        graphics_renderer_draw_pixel (renderer, x0, y, r_1, g_1, b_1);
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

void graphics_renderer_draw_shaded_triangle (graphics_renderer_t *renderer, int x0, int y0, int x1, int y1, int x2, int y2, uint8_t r_0, uint8_t g_0, uint8_t b_0, uint8_t r_1, uint8_t g_1, uint8_t b_1, uint8_t r_2, uint8_t g_2, uint8_t b_2) {
    /* Sort the points in vertical order */
    if (y1 < y0) {
        swap_int (&x0, &x1);
        swap_int (&y0, &y1);
        swap_uint_8 (&r_0, &r_1);
        swap_uint_8 (&g_0, &g_1);
        swap_uint_8 (&b_0, &b_1);
    }

    if (y2 < y0) {
        swap_int (&x0, &x2);
        swap_int (&y0, &y2);
        swap_uint_8 (&r_0, &r_2);
        swap_uint_8 (&g_0, &g_2);
        swap_uint_8 (&b_0, &b_2);
    }

    if (y2 < y1) {
        swap_int (&x1, &x2);
        swap_int (&y1, &y2);
        swap_uint_8 (&r_1, &r_2);
        swap_uint_8 (&g_1, &g_2);
        swap_uint_8 (&b_1, &b_2);
    }

    /* Compute change in x per unit y for each line 
       segment. */
    int dy_0_1 = y1 - y0;
    int dy_0_2 = y2 - y0;
    int dy_1_2 = y2 - y1;

    /* Render first half of triangle: (x0, y0) -> (x1, y1) and (x', y1)*/
    double p1_x = x0;
    double p2_x = x0;
    double p1_r = r_0;
    double p2_r = r_0;
    double p1_g = g_0;
    double p2_g = g_0;
    double p1_b = b_0;
    double p2_b = b_0;
        
    if (y1 > y0) {
        /* Change in x per unit y */
        double x_0_1 = (x1 - x0) / (double) dy_0_1;
        double x_0_2 = (x2 - x0) / (double) dy_0_2;

        /* Change in colours pre unit y */
        double r_0_1 = (r_1 - r_0) / (double) dy_0_1;
        double r_0_2 = (r_2 - r_0) / (double) dy_0_2;
        double g_0_1 = (g_1 - g_0) / (double) dy_0_1;
        double g_0_2 = (g_2 - g_0) / (double) dy_0_2;
        double b_0_1 = (b_1 - b_0) / (double) dy_0_1;
        double b_0_2 = (b_2 - b_0) / (double) dy_0_2;
    
        for (int i = y0; i < y1; i++) {
            draw_horizontal_line_shaded (renderer, (int) p1_x, (int) p2_x, i, (uint8_t) p1_r, (uint8_t) p1_g, (uint8_t) p1_b, (uint8_t) p2_r, (uint8_t) p2_g, (uint8_t) p2_b);
            
            p1_x += x_0_1;
            p1_r += r_0_1;
            p1_g += g_0_1;
            p1_b += b_0_1;

            p2_x += x_0_2;
            p2_r += r_0_2;
            p2_g += g_0_2;
            p2_b += b_0_2;
        }
    }

    /* Draw second half of triangle (x1, y1) and (x', y1) -> (x2, y2) */
    p1_x = x1;
    p1_r = r_1;
    p1_g = g_1;
    p1_b = b_1;

    if (y2 > y1) {
        /* Change in x per unit y */
        double x_1_2 = (x2 - x1) / (double) dy_1_2;
        double x_0_2 = (x2 - x0) / (double) dy_0_2;

        /* Change in colours per unit y */
        double r_1_2 = (r_2 - r_1) / (double) dy_1_2;
        double r_0_2 = (r_2 - r_0) / (double) dy_0_2;
        double g_1_2 = (g_2 - g_1) / (double) dy_1_2;
        double g_0_2 = (g_2 - g_0) / (double) dy_0_2;
        double b_1_2 = (b_2 - b_1) / (double) dy_1_2;
        double b_0_2 = (b_2 - b_0) / (double) dy_0_2;
    
        for (int i = y1; i <= y2; i++) {
            draw_horizontal_line_shaded (renderer, (int) p1_x, (int) p2_x, i, (uint8_t) p1_r, (uint8_t) p1_g, (uint8_t) p1_b, (uint8_t) p2_r, (uint8_t) p2_g, (uint8_t) p2_b);
            
            p1_x += x_1_2;
            p1_r += r_1_2;
            p1_g += g_1_2;
            p1_b += b_1_2;

            p2_x += x_0_2;
            p2_r += r_0_2;
            p2_g += g_0_2;
            p2_b += b_0_2;
        }
    } else {
        printf ("(%f, %f, %f) (%d, %d, %d)\n", p2_r, p2_g, p2_b, r_2, g_2, b_2);
        draw_horizontal_line_shaded (renderer, (int) p1_x, (int) p2_x, y1, p1_r, p1_g, p1_b, p2_r, p2_g, p2_b);
            
        // draw_horizontal_line_shaded (renderer, (int) p1_x, (int) p2_x, y1, p1_r, p1_g, p1_b, p2_r, p2_g, p2_b);
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
