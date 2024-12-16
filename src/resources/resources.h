#ifndef RESOURCES_H
#define RESOURCES_H

#include "./../maths/maths.h"

typedef struct {
    maths_vec4f coord;
} resources_vertex_t;

typedef int resources_triangle_t[3];

typedef struct {
    resources_vertex_t *vertices;
    resources_triangle_t *faces;
    int num_vertices;
    int num_faces;
} resources_mesh_t;

typedef struct {
    resources_mesh_t *mesh;
    maths_vec4f position;
    maths_vec4f scale;
    maths_vec4f rotation;
} resources_model_t;

resources_mesh_t *resources_load_mesh_from_obj_file (const char *file_name);

#endif