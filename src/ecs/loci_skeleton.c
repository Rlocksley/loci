#include "loci_skeleton.h"


void loci_createSkeleton(ecs_entity_t entity, 
Loci_SkeletonVertex* vertices, uint32_t numberVertices, 
mat4 transform)
{
    const Loci_Mesh* pMesh;
    loci_get(entity, Loci_Mesh, pMesh);

    Loci_Skeleton skeleton;

    Loci_BufferInterface vertexInterface = 
    loci_createBufferInterface
    (numberVertices, sizeof(Loci_SkeletonVertex),
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    memcpy(vertexInterface.pMemory, vertices, numberVertices*sizeof(Loci_SkeletonVertex));

    skeleton.vertexBuffer = 
    loci_createBuffer
    (numberVertices, sizeof(Loci_SkeletonVertex),
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
    VK_BUFFER_USAGE_TRANSFER_DST_BIT);

    VkBufferCopy copy;
    copy.srcOffset = 0;
    copy.dstOffset = 0;
    copy.size = numberVertices*sizeof(Loci_SkeletonVertex);

    loci_beginCommand(loci_buildVkCommandBuffer);

    vkCmdCopyBuffer
    (loci_buildVkCommandBuffer,
    vertexInterface.vkBuffer,
    skeleton.vertexBuffer.vkBuffer,
    1, &copy);

    loci_endCommand(loci_buildVkCommandBuffer);

    loci_submitSingleCommand(loci_buildVkCommandBuffer, loci_buildVkFence);

    loci_destroyBufferInterface
    (vertexInterface);

    skeleton.bonesBuffer = 
    loci_createBufferInterface
    (1, sizeof(Loci_Bones),
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);


    skeleton.descriptorSet = 
    loci_createSkeletonDescriptorSet
    (pMesh->vertexBuffer, 
    skeleton.vertexBuffer, skeleton.bonesBuffer);

    skeleton.pipeline = 
    loci_createSkeletonPipeline
    (skeleton.descriptorSet);

    loci_set(entity, Loci_Skeleton, skeleton);

}

void loci_destroySkeleton(ecs_entity_t entity)
{
    const Loci_Skeleton* pSkeleton;
    loci_get(entity, Loci_Skeleton, pSkeleton)
    Loci_Skeleton skeleton = *pSkeleton;

    loci_destroySkeletonPipeline
    (skeleton.pipeline);

    loci_destroySkeletonDescriptorSet
    (skeleton.descriptorSet);

    loci_destroyBufferInterface
    (skeleton.bonesBuffer);

    loci_destroyBuffer
    (skeleton.vertexBuffer);
}