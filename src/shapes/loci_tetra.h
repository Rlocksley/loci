#ifndef LOCI_TETRA_H
#define LOCI_TETRA_H

#include "../ecs/loci_mesh.h"

typedef struct Loci_Tetra
{
    Loci_Vertex vertices[4];
    uint32_t indices[12];
}Loci_Tetra;

Loci_Tetra loci_createTetra();

#endif