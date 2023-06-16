#ifndef LOCI_CUBE_H
#define LOCI_CUBE_H

#include "../ecs/loci_mesh.h"


typedef struct Loci_Cube
{
    uint32_t numberVertices;
    Loci_Vertex* pVertices;
    uint32_t numberIndices;
    uint32_t* pIndices;
}Loci_Cube;

Loci_Cube loci_createCube(float width, float height, float depth, vec3 color);
void loci_destroyCube(Loci_Cube cube);

#endif