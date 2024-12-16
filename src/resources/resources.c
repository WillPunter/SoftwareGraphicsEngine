 #include "resources.h"
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

resources_mesh_t *resources_load_mesh_from_obj_file (const char *file_name) {
    /* Open file */
    FILE *obj_file = fopen (file_name, "r");

    resources_mesh_t *result;

    if (obj_file) {
        result = (resources_mesh_t *) malloc (sizeof (resources_mesh_t));

        if (result == NULL) {
            fprintf (
                stderr,
                "Error - resources/load_mesh_from_obj_file:"
                " could not allocate memory for mesh structure/n"
            );

            fclose (obj_file);
            return NULL;
        }

        /* Count number of vertices and faces
           so that buffers can be allocated for
           these within mesh before parsing
           of the file takes place. */
        result->num_vertices = 0;
        result->num_faces = 0;

        char *buff = NULL;
        size_t len = 0;

        while (getline (&buff, &len, obj_file) != -1) {
            /* Check if a vertex or face */
            if (strlen (buff) > 1) {
                if (buff[0] == 'v' && buff[1] == ' ') {
                    result->num_vertices ++;
                } else if (buff[0] == 'f' && buff[1] == ' ') {
                    result->num_faces ++;
                }
            }
        }

        printf ("done with pass 1\n");

        /* Allocate number of vertices and faces */
        result->vertices = (resources_vertex_t *) malloc (sizeof (resources_vertex_t) * result->num_vertices);

        if (result->vertices == NULL) {
            fprintf (
                stderr,
                "Error - resources/load_mesh_from_obj_file:"
                " could not allocate memory for vertices."
            );

            if (buff) {
                free (buff);
            }

            free (result);

            fclose (obj_file);

            return NULL;
        }

        result->faces = (resources_triangle_t *) malloc (sizeof (resources_triangle_t) * result->num_faces);

        if (result->faces == NULL) {
            fprintf (
                stderr,
                "Error - resources/load_mesh_from_obj_file:"
                " could not allocate memory for faces."
            );

            if (buff) {
                free (buff);
            }

            free (result->faces);
            free (result);

            fclose (obj_file);

            return NULL;
        }

        /* Second pass */
        rewind (obj_file);

        int vertex_index = 0;
        int face_index = 0;

        while (getline (&buff, &len, obj_file) != -1) {
            if (strlen (buff) > 2) {
                if (buff[0] == 'v' && buff[1] == ' ') {
                    /* Add vertex */
                    maths_vec4f coord;
                    coord.w = 1;

                    if (sscanf (buff + 2, "%lf %lf %lf", &coord.x, &coord.y, &coord.z) == 3) {
                        result->vertices[vertex_index].coord = coord;
                        vertex_index ++;
                    }
                } else if (buff[0] == 'f' && buff[1] == ' ') {
                    /* Add face */
                    int i1;
                    int t1;
                    int i2;
                    int t2;
                    int i3;
                    int t3;

                    if (sscanf (buff + 2, "%d/%d %d/%d %d/%d", &i1, &t1, &i2, &t2, &i3, &t3) == 6) {
                        result->faces[face_index][0] = i1 - 1;
                        result->faces[face_index][1] = i2 - 1;
                        result->faces[face_index][2] = i3 - 1;
                        face_index ++;
                    }

                    printf ("New face: %d %d %d\n", i1, i2, i3);
                }
            }
        }

        if (buff) {
            free (buff);
        }

        fclose (obj_file);

        return result;
    } else {
        fprintf (
            stderr,
            "Error - resources/load_mesh_from_obj_file:"
            "could not open file %s/n",
            file_name
        );

        return NULL;
    }
}