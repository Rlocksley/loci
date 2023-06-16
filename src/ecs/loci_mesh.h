#ifndef LOCI_BUFFER_H
#define LOCI_BUFFER_H
#include "../core/loci_buffer.h"
#endif
#ifndef LOCI_ACCELERATION_H
#define LOCI_ACCELERATION_H
#include "../core/loci_acceleration.h"
#endif
#ifndef LOCI_COMMANDBUFFER_H
#define LOCI_COMMANDBUFFER_H
#include "../core/loci_commandBuffer.h"
#endif
#ifndef LOCI_WORLD_H
#define LOCI_WORLD_H
#include "../core/loci_world.h"
#endif

typedef struct Loci_Vertex
{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;
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


void loci_createMesh(ecs_entity_t entity, 
Loci_Vertex* vertices, uint32_t numberVertices, 
uint32_t* indices, uint32_t numberIndices,
mat4 transform)
{
    Loci_BufferInterface vertexInterface =
    loci_createBufferInterface
    (numberVertices, sizeof(Loci_Vertex),
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    memcpy(vertexInterface.pMemory, vertices, numberVertices*sizeof(Loci_Vertex));

    Loci_BufferInterface indexInterface = 
    loci_createBufferInterface
    (numberIndices, sizeof(uint32_t),
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    memcpy(indexInterface.pMemory, indices, numberIndices*sizeof(uint32_t));

    VkTransformMatrixKHR matrix;
    for(uint32_t i = 0; i < 3; i++)
    {
        for(uint32_t j = 0; j < 4; j++)
        {
            matrix.matrix[i][j] = transform[j][i];
        }
    }

    Loci_BufferInterface transformInterface = 
    loci_createBufferInterface
    (1, sizeof(VkTransformMatrixKHR),
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    memcpy(transformInterface.pMemory, &matrix, sizeof(VkTransformMatrixKHR));


    Loci_Mesh mesh;
    
    mesh.vertexBuffer = 
    loci_createBuffer
    (numberVertices, sizeof(Loci_Vertex),
    VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR |
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    mesh.indexBuffer = 
    loci_createBuffer
    (numberIndices, sizeof(uint32_t),
    VK_BUFFER_USAGE_TRANSFER_DST_BIT |
    VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR |
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    mesh.transformBuffer = 
    loci_createBuffer
    (1, sizeof(VkTransformMatrixKHR),
    VK_BUFFER_USAGE_TRANSFER_DST_BIT |   
    VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR);

    VkBufferCopy vertexCopy;
    vertexCopy.srcOffset = 0;
    vertexCopy.dstOffset = 0;
    vertexCopy.size = numberVertices*sizeof(Loci_Vertex);

    VkBufferCopy indexCopy;
    indexCopy.srcOffset = 0;
    indexCopy.dstOffset = 0;
    indexCopy.size = numberIndices*sizeof(uint32_t);

    VkBufferCopy transformCopy;
    transformCopy.srcOffset = 0;
    transformCopy.dstOffset = 0;
    transformCopy.size = sizeof(VkTransformMatrixKHR);

    loci_beginCommand(loci_buildVkCommandBuffer);

    vkCmdCopyBuffer
    (loci_buildVkCommandBuffer,
    vertexInterface.vkBuffer,
    mesh.vertexBuffer.vkBuffer,
    1, &vertexCopy);

    vkCmdCopyBuffer
    (loci_buildVkCommandBuffer,
    indexInterface.vkBuffer,
    mesh.indexBuffer.vkBuffer,
    1, &indexCopy);

    vkCmdCopyBuffer
    (loci_buildVkCommandBuffer,
    transformInterface.vkBuffer,
    mesh.transformBuffer.vkBuffer,
    1, &transformCopy);

    loci_endCommand(loci_buildVkCommandBuffer);

    loci_submitSingleCommand
    (loci_buildVkCommandBuffer, loci_buildVkFence);

    mesh.acceleration = 
    loci_createBottomAcceleration
    (mesh.vertexBuffer, mesh.indexBuffer,
    mesh.transformBuffer);

    loci_destroyBufferInterface(transformInterface);
    loci_destroyBufferInterface(indexInterface);
    loci_destroyBufferInterface(vertexInterface);

    loci_set(entity, Loci_Mesh, mesh);
}

void loci_destroyMesh(ecs_entity_t entity)
{
    const Loci_Mesh* pMesh;
    loci_get(entity, Loci_Mesh, pMesh);
    Loci_Mesh mesh = *pMesh;

    loci_destroyBottomAcceleration
    (mesh.acceleration);

    loci_destroyBuffer
    (mesh.transformBuffer);

    loci_destroyBuffer
    (mesh.indexBuffer);

    loci_destroyBuffer
    (mesh.vertexBuffer);

    loci_remove(entity, Loci_Mesh);
}