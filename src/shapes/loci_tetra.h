#ifndef LOCI_MESH_H
#define LOCI_MESH_H
#include "../ecs/loci_mesh.h"
#endif

typedef struct Loci_Tetra
{
    Loci_Vertex vertices[4];
    uint32_t indices[12];
}Loci_Tetra;


Loci_Tetra loci_createTetra()
{
    Loci_Tetra tetra;


    glm_vec3_copy((vec3){0.f,0.f,0.f},tetra.vertices[0].position);
    glm_vec3_copy((vec3){1.f,0.f,0.f},tetra.vertices[1].position);
    glm_vec3_copy((vec3){0.5f,1.f,0.f},tetra.vertices[2].position);
    glm_vec3_copy((vec3){0.5f,0.5f,sqrtf(2)/sqrtf(3)},tetra.vertices[3].position);

    glm_vec3_copy((vec3){0.f,0.f,0.f},tetra.vertices[0].normal);
    glm_vec3_copy((vec3){1.f,0.f,0.f},tetra.vertices[1].normal);
    glm_vec3_copy((vec3){0.5f,1.f,0.f},tetra.vertices[2].normal);
    glm_vec3_copy((vec3){0.5f,0.5f,sqrtf(2)/sqrtf(3)},tetra.vertices[3].normal);

    glm_vec2_copy((vec3){0.f,0.f},tetra.vertices[0].uv);
    glm_vec2_copy((vec3){1.f,0.f},tetra.vertices[1].uv);
    glm_vec2_copy((vec3){0.5f,1.f},tetra.vertices[2].uv);
    glm_vec2_copy((vec3){0.5f,0.5f},tetra.vertices[3].uv);


    tetra.indices[0] = 0;
    tetra.indices[1] = 1;
    tetra.indices[2] = 2;
    tetra.indices[3] = 0;
    tetra.indices[4] = 2;
    tetra.indices[5] = 3;
    tetra.indices[6] = 0;
    tetra.indices[7] = 3;
    tetra.indices[8] = 1;
    tetra.indices[9] = 1;
    tetra.indices[10] = 3;
    tetra.indices[11] = 2;

    return tetra;
};