#ifndef MATHS_H
#define MATHS_H

#include <math.h>
#include <stdio.h>
#include <string.h>

/* Vectors */
typedef struct {
    double x;
    double y;
} maths_vec2f;

typedef struct {
    double x;
    double y;
    double z;
} maths_vec3f;

typedef struct {
    double x;
    double y;
    double z;
    double w;
} maths_vec4f;

/* Matrices 
    
   Note that, for consistency with other
   mathematics libraries and renderers,
   matrices are accessed in column-major
   order, i.e. we access elements using
   mat[col][row] instead of mat[row][col]. */

typedef struct {
    double data[2][2];
} maths_mat2x2f;

typedef struct {
    double data[3][3];
} maths_mat3x3f;

typedef struct {
    double data[4][4];
} maths_mat4x4f;

/* Vector Functions */
inline maths_vec2f maths_vec2f_add (maths_vec2f a, maths_vec2f b) {
    return (maths_vec2f) { a.x + b.x, a.y + b.y };
}

inline maths_vec3f maths_vec3f_add (maths_vec3f a, maths_vec3f b) {
    return (maths_vec3f) { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline maths_vec4f maths_vec4f_add (maths_vec4f a, maths_vec4f b) {
    return (maths_vec4f) { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline maths_vec2f maths_vec2f_sub (maths_vec2f a, maths_vec2f b) {
    return (maths_vec2f) { a.x - b.x, a.y - b.y };
}

inline maths_vec3f maths_vec3f_sub (maths_vec3f a, maths_vec3f b) {
    return (maths_vec3f) { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline maths_vec4f maths_vec4f_sub (maths_vec4f a, maths_vec4f b) {
    return (maths_vec4f) { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

inline double maths_vec2f_dot (maths_vec2f a, maths_vec2f b) {
    return a.x * b.x + a.y * b.y;
}

inline double maths_vec3f_dot (maths_vec3f a, maths_vec3f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline double maths_vec4f_dot (maths_vec4f a, maths_vec4f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline double maths_vec2f_magnitude (maths_vec2f a) {
    return sqrt (a.x * a.x + a.y * a.y);
}

inline double maths_vec3f_magnitude (maths_vec3f a) {
    return sqrt (a.x * a.x + a.y * a.y + a.z * a.z);
}

inline double maths_vec4f_magnitude (maths_vec4f a) {
    return sqrt (a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

inline maths_vec2f maths_vec2f_scale (maths_vec2f a, double scale) {
    return (maths_vec2f) { a.x * scale, a.y * scale };
}

inline maths_vec3f maths_vec3f_scale (maths_vec3f a, double scale) {
    return (maths_vec3f) { a.x * scale, a.y * scale, a.z * scale };
}

inline maths_vec4f maths_vec4f_scale (maths_vec4f a, double scale) {
    return (maths_vec4f) { a.x * scale, a.y * scale, a.z * scale, a.w * scale };
}

inline maths_vec2f maths_vec2f_normalise (maths_vec2f a) {
    double mag = maths_vec2f_magnitude (a);
    return (maths_vec2f) { a.x / mag, a.y / mag };
}

inline maths_vec3f maths_vec3f_normalise (maths_vec3f a) {
    double mag = maths_vec3f_magnitude (a);
    return (maths_vec3f) { a.x / mag, a.y / mag, a.z / mag };
}

inline maths_vec4f maths_vec4f_normalise (maths_vec4f a) {
    double mag = maths_vec4f_magnitude (a);
    return (maths_vec4f) { a.x / mag, a.y / mag, a.z / mag, a.w / mag };
}

inline maths_vec3f maths_vec3f_cross (maths_vec3f a, maths_vec3f b) {
    return (maths_vec3f) {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

inline maths_vec4f maths_vec4f_cross_3d (maths_vec4f a, maths_vec4f b) {
    return (maths_vec4f) {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x,
        0.f
    };
}

/* Matrix operations */
inline maths_mat2x2f maths_mat2x2f_add (maths_mat2x2f a, maths_mat2x2f b) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            a.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }

    return a;
}

inline maths_mat3x3f maths_mat3x3f_add (maths_mat3x3f a, maths_mat3x3f b) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    
    return a;
}

inline maths_mat4x4f maths_mat4x4f_add (maths_mat4x4f a, maths_mat4x4f b) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            a.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }
    
    return a;
}

inline maths_mat2x2f maths_mat2x2f_sub (maths_mat2x2f a, maths_mat2x2f b) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            a.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }

    return a;
}

inline maths_mat3x3f maths_mat3x3f_sub (maths_mat3x3f a, maths_mat3x3f b) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }
    
    return a;
}

inline maths_mat4x4f maths_mat4x4f_sub (maths_mat4x4f a, maths_mat4x4f b) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            a.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }
    
    return a;
}

inline maths_vec2f maths_mat2x2f_mul_vec2f (maths_mat2x2f m, maths_vec2f v) {
    return (maths_vec2f) {
        m.data [0][0] * v.x + m.data[1][0] * v.y, 
        m.data [0][1] * v.x + m.data[1][1] * v.y
    };
}

inline maths_vec3f maths_mat3x3f_mul_vec3f (maths_mat3x3f m, maths_vec3f v) {
    return (maths_vec3f) {
        m.data [0][0] * v.x + m.data[1][0] * v.y + m.data[2][0] * v.z, 
        m.data [0][1] * v.x + m.data[1][1] * v.y + m.data[2][1] * v.z,
        m.data [0][2] * v.x + m.data[1][2] * v.y + m.data[2][2] * v.z
    };
}

inline maths_vec4f maths_mat4x4f_mul_vec4f (maths_mat4x4f m, maths_vec4f v) {
    return (maths_vec4f) {
        m.data [0][0] * v.x + m.data[1][0] * v.y + m.data[2][0] * v.z + m.data[3][0] * v.w, 
        m.data [0][1] * v.x + m.data[1][1] * v.y + m.data[2][1] * v.z + m.data[3][1] * v.w,
        m.data [0][2] * v.x + m.data[1][2] * v.y + m.data[2][2] * v.z + m.data[3][2] * v.w,
        m.data [0][3] * v.x + m.data[1][3] * v.y + m.data[2][3] * v.z + m.data[3][3] * v.w
    };
}

inline maths_mat2x2f maths_mat2x2f_mul (maths_mat2x2f a, maths_mat2x2f b) {
    maths_mat2x2f result;

    /* i = column, j = row */
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result.data[i][j] =
                a.data[0][j] * b.data[i][0] +
                a.data[1][j] * b.data[i][1]; 
        }
    }

    return result;
}

inline maths_mat3x3f maths_mat3x3f_mul (maths_mat3x3f a, maths_mat3x3f b) {
    maths_mat3x3f result;

    /* i = column, j = row */
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.data[i][j] =
                a.data[0][j] * b.data[i][0] +
                a.data[1][j] * b.data[i][1] +
                a.data[2][j] * b.data[i][2]; 
        }
    }

    return result;
}

inline maths_mat4x4f maths_mat4x4f_mul (maths_mat4x4f a, maths_mat4x4f b) {
    maths_mat4x4f result;

    /* i = column, j = row */
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] =
                a.data[0][j] * b.data[i][0] +
                a.data[1][j] * b.data[i][1] +
                a.data[2][j] * b.data[i][2] +
                a.data[3][j] * b.data[i][3];
        }
    }

    return result;
}

inline maths_mat2x2f maths_mat2x2f_transpose (maths_mat2x2f a) {
    maths_mat2x2f result;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result.data[i][j] = a.data[j][i];
        }
    }

    return result;
}

inline maths_mat3x3f maths_mat3x3f_transpose (maths_mat3x3f a) {
    maths_mat3x3f result;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.data[i][j] = a.data[j][i];
        }
    }

    return result;
}

inline maths_mat4x4f maths_mat4x4f_transpose (maths_mat4x4f a) {
    maths_mat4x4f result;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.data[i][j] = a.data[j][i];
        }
    }

    return result;
}

/* Transformation Matrices */
inline maths_mat2x2f maths_mat2x2f_identity () {
    maths_mat2x2f result;
    memset (&result, 0, sizeof (maths_mat2x2f));
    
    result.data[0][0] = 1;
    result.data[1][1] = 1;

    return result;
}

inline maths_mat3x3f maths_mat3x3f_identity () {
    maths_mat3x3f result;
    memset (&result, 0, sizeof (maths_mat3x3f));
    
    result.data[0][0] = 1;
    result.data[1][1] = 1;
    result.data[2][2] = 1;

    return result;
}

inline maths_mat4x4f maths_mat4x4f_identity () {
    maths_mat4x4f result;
    memset (&result, 0, sizeof (maths_mat4x4f));
    
    result.data[0][0] = 1;
    result.data[1][1] = 1;
    result.data[2][2] = 1;
    result.data[3][3] = 1;

    return result;
}

inline maths_mat2x2f maths_mat2x2f_enlargement (double x, double y) {
    maths_mat2x2f result;
    memset (&result, 0, sizeof (maths_mat2x2f));
    
    result.data[0][0] = x;
    result.data[1][1] = y;

    return result;
}

inline maths_mat3x3f maths_mat3x3f_enlargement (double x, double y, double z) {
    maths_mat3x3f result;
    memset (&result, 0, sizeof (maths_mat3x3f));
    
    result.data[0][0] = x;
    result.data[1][1] = y;
    result.data[2][2] = z;

    return result;
}

inline maths_mat4x4f maths_mat4x4f_enlargment (double x, double y, double z, double w) {
    maths_mat4x4f result;
    memset (&result, 0, sizeof (maths_mat4x4f));
    
    result.data[0][0] = x;
    result.data[1][1] = y;
    result.data[2][2] = z;
    result.data[3][3] = w;

    return result;
}

/* All rotations are done in radians. */

inline maths_mat2x2f maths_mat2x2f_rotation (double angle) {
    maths_mat2x2f result;
    
    result.data[0][0] = cos (angle);
    result.data[1][0] = -sin (angle);
    result.data[0][1] = sin (angle);
    result.data[1][1] = cos (angle);

    return result;
}

/* 3x3 rotation around x-axis */
inline maths_mat3x3f maths_3x3f_rotation_x (double angle) {
    maths_mat3x3f result = maths_mat3x3f_identity ();

    result.data[2][2] = cos(angle);
    result.data[1][2] = -sin(angle);
    result.data[2][1] = sin (angle);
    result.data[1][1] = cos(angle);

    return result; 
}

/* 3x3 rotation around y-axis */
inline maths_mat3x3f maths_3x3f_rotation_y (double angle) {
    maths_mat3x3f result = maths_mat3x3f_identity ();

    result.data[0][0] = cos(angle);
    result.data[2][0] = -sin(angle);
    result.data[0][2] = sin (angle);
    result.data[2][2] = cos(angle);

    return result; 
}

/* 3x3 rotation around z-axis */
inline maths_mat3x3f maths_3x3f_rotation_z (double angle) {
    maths_mat3x3f result = maths_mat3x3f_identity ();

    result.data[0][0] = cos(angle);
    result.data[1][0] = -sin(angle);
    result.data[0][1] = sin (angle);
    result.data[1][1] = cos(angle);

    return result; 
}

/* 4x4 rotation around x-axis */
inline maths_mat4x4f maths_4x4f_rotation_x_3d (double angle) {
    maths_mat4x4f result = maths_mat4x4f_identity ();

    result.data[2][2] = cos(angle);
    result.data[1][2] = -sin(angle);
    result.data[2][1] = sin (angle);
    result.data[1][1] = cos(angle);

    return result; 
}

/* 4x4 rotation around y-axis */
inline maths_mat4x4f maths_4x4f_rotation_y_3d (double angle) {
    maths_mat4x4f result = maths_mat4x4f_identity ();

    result.data[0][0] = cos(angle);
    result.data[2][0] = -sin(angle);
    result.data[0][2] = sin (angle);
    result.data[2][2] = cos(angle);

    return result; 
}

/* 3x3 rotation around z-axis */
inline maths_mat4x4f maths_4x4f_rotation_z_3d (double angle) {
    maths_mat4x4f result = maths_mat4x4f_identity ();

    result.data[0][0] = cos(angle);
    result.data[1][0] = -sin(angle);
    result.data[0][1] = sin (angle);
    result.data[1][1] = cos(angle);

    return result; 
}

maths_vec2f maths_project_vertex_3f (double viewing_plane_distance, int buffer_width, int buffer_height, double view_width, double view_height, maths_vec3f v);

#endif