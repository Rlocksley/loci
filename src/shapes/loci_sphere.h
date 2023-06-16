#ifndef LOCI_SPHERE_H
#define LOCI_SPHERE_H

#include "../ecs/loci_mesh.h"

typedef struct Loci_Sphere
{
    uint32_t numberVertices;
    Loci_Vertex* pVertices;
    uint32_t numberIndices;
    uint32_t* pIndices;
}Loci_Sphere;

Loci_Sphere loci_createSphereOut(float radius, uint32_t numSegments, vec3 color);
Loci_Sphere loci_createSphereIn(float radius, uint32_t numSegments);
void loci_destroySphere(Loci_Sphere sphere);

#endif