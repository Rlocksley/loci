#include "loci_sphere.h"

Loci_Sphere loci_createSphereOut(float radius, uint32_t numSegments, vec3 color) 
{
    
    Loci_Sphere sphere;

    unsigned int numRings = numSegments / 2;

    uint32_t numVertices = (numRings + 1) * (numSegments + 1);
    uint32_t numIndices = numRings * numSegments * 6;

    sphere.numberVertices = numVertices;
    sphere.numberIndices = numIndices;
    sphere.pVertices = (Loci_Vertex*)malloc(numVertices * sizeof(Loci_Vertex));
    sphere.pIndices = (uint32_t*)malloc(numIndices * sizeof(uint32_t));

    float ringStep = GLM_PI / numRings;
    float segmentStep = 2.0f * GLM_PI / numSegments;

    unsigned int index = 0;

    for (unsigned int ring = 0; ring <= numRings; ring++) {
        float theta = ring * ringStep;
        float y = radius * cos(theta);
        float sinTheta = sin(theta);

        for (unsigned int segment = 0; segment <= numSegments; segment++) {
            float phi = segment * segmentStep;
            float x = radius * sinTheta * cos(phi);
            float z = radius * sinTheta * sin(phi);

            sphere.pVertices[index].position[0] = x;
            sphere.pVertices[index].position[1] = y;
            sphere.pVertices[index].position[2] = z;

            sphere.pVertices[index].normal[0] = x / radius;
            sphere.pVertices[index].normal[1] = y / radius;
            sphere.pVertices[index].normal[2] = z / radius;

            glm_vec3_copy(color,sphere.pVertices[index].color);

            sphere.pVertices[index].uv[0] = (float)segment / numSegments;
            sphere.pVertices[index].uv[1] = (float)ring / numRings;

            index++;
        }
    }

    index = 0;

    for (unsigned int ring = 0; ring < numRings; ring++) {
        for (unsigned int segment = 0; segment < numSegments; segment++) {
            unsigned int index0 = ring * (numSegments + 1) + segment;
            unsigned int index1 = index0 + 1;
            unsigned int index2 = (ring + 1) * (numSegments + 1) + segment;
            unsigned int index3 = index2 + 1;

            sphere.pIndices[index++] = index0;
            sphere.pIndices[index++] = index2;
            sphere.pIndices[index++] = index1;
            sphere.pIndices[index++] = index1;
            sphere.pIndices[index++] = index2;
            sphere.pIndices[index++] = index3;
        }
    }

    return sphere;
}

Loci_Sphere loci_createSphereIn(float radius, uint32_t numSegments) 
{
    
    Loci_Sphere sphere;

    unsigned int numRings = numSegments / 2;

    uint32_t numVertices = (numRings + 1) * (numSegments + 1);
    uint32_t numIndices = numRings * numSegments * 6;

    sphere.numberVertices = numVertices;
    sphere.numberIndices = numIndices;
    sphere.pVertices = (Loci_Vertex*)malloc(numVertices * sizeof(Loci_Vertex));
    sphere.pIndices = (uint32_t*)malloc(numIndices * sizeof(uint32_t));

    float ringStep = GLM_PI / numRings;
    float segmentStep = 2.0f * GLM_PI / numSegments;

    unsigned int index = 0;

    for (unsigned int ring = 0; ring <= numRings; ring++) {
        float theta = ring * ringStep;
        float y = radius * cos(theta);
        float sinTheta = sin(theta);

        for (unsigned int segment = 0; segment <= numSegments; segment++) {
            float phi = segment * segmentStep;
            float x = radius * sinTheta * cos(phi);
            float z = radius * sinTheta * sin(phi);

            sphere.pVertices[index].position[0] = x;
            sphere.pVertices[index].position[1] = y;
            sphere.pVertices[index].position[2] = z;

            sphere.pVertices[index].normal[0] = -x / radius;
            sphere.pVertices[index].normal[1] = -y / radius;
            sphere.pVertices[index].normal[2] = -z / radius;

            sphere.pVertices[index].uv[0] = (float)segment / numSegments;
            sphere.pVertices[index].uv[1] = (float)ring / numRings;

            index++;
        }
    }

    index = 0;

    for (unsigned int ring = 0; ring < numRings; ring++) {
        for (unsigned int segment = 0; segment < numSegments; segment++) {
            unsigned int index0 = ring * (numSegments + 1) + segment;
            unsigned int index1 = index0 + 1;
            unsigned int index2 = (ring + 1) * (numSegments + 1) + segment;
            unsigned int index3 = index2 + 1;

            sphere.pIndices[index++] = index0;
            sphere.pIndices[index++] = index2;
            sphere.pIndices[index++] = index1;
            sphere.pIndices[index++] = index1;
            sphere.pIndices[index++] = index2;
            sphere.pIndices[index++] = index3;
        }
    }

    return sphere;
}

void loci_destroySphere(Loci_Sphere sphere)
{
    free(sphere.pIndices);
    free(sphere.pVertices);
}