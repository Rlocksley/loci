#ifndef LOCI_SKELETON_H
#define LOCI_SKELETON_H

#include "../core/loci_buffer.h"
#include "../core/loci_semaphore.h"
#include "../core/loci_commandBuffer.h"
#include "../core/loci_descriptorSet.h"
#include "../core/loci_pipeline.h"
#include "../core/loci_world.h"
#include "loci_mesh.h"

typedef struct Loci_SkeletonVertex
{
    vec4 position;
    vec4 normal;
    ivec4 numberBones;
    vec4 weights;
    ivec4 boneNumbers;
}Loci_SkeletonVertex;

typedef struct Loci_Bones
{
    mat4 bones[256];
}Loci_Bones;

typedef struct Loci_Skeleton
{
    Loci_Buffer vertexBuffer;
    Loci_BufferInterface bonesBuffer;
    Loci_SkeletonDescriptorSet descriptorSet;
    Loci_SkeletonPipeline pipeline;
}Loci_Skeleton;

void loci_createSkeleton(ecs_entity_t entity, 
Loci_SkeletonVertex* vertices, uint32_t numberVertices, 
mat4 transform);
void loci_destroySkeleton(ecs_entity_t entity);

#endif