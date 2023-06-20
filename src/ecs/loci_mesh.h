#ifndef LOCI_MESH_H
#define LOCI_MESH_H

#include "../core/loci_buffer.h"
#include "../core/loci_acceleration.h"
#include "../core/loci_commandBuffer.h"
#include "../core/loci_world.h"

typedef struct Loci_Vertex
{
    vec4 position;
    vec4 normal;
    vec4 color;
    vec4 uv;
}Loci_Vertex;



typedef struct Loci_Mesh
{
    Loci_Buffer vertexBuffer;
    Loci_Buffer indexBuffer;
    Loci_Buffer transformBuffer;
    Loci_BottomAcceleration acceleration;
}Loci_Mesh;

void loci_createMesh(ecs_entity_t entity, 
Loci_Vertex* vertices, uint32_t numberVertices, 
uint32_t* indices, uint32_t numberIndices,
mat4 transform);
void loci_destroyMesh(ecs_entity_t entity);

#endif