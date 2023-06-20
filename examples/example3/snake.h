#ifndef LOCI_SNAKE_H
#define LOCI_SNAKE_H

#include "../../shapes/loci_cube.h"
#include "../../ecs/loci_mesh.h"
#include "../../ecs/loci_skeleton.h"
#include "../../ecs/loci_bone.h"
#include "../../ecs/loci_channel.h"

typedef struct SnakeMesh
{
    uint32_t numberVertices;
    Loci_Vertex* pVertices;
    uint32_t numberIndices;
    uint32_t* pIndices;
}SnakeMesh;

SnakeMesh createSnakeMesh
(uint32_t numberSections, 
float space, float width, float height, float depth, vec3 color);
void destroySnakeMesh(SnakeMesh mesh);

typedef struct SnakeSkeleton
{
    uint32_t numberVertices;
    Loci_SkeletonVertex* pVertices;
}SnakeSkeleton;

SnakeSkeleton createSnakeSkeleton
(uint32_t numberSections, 
float space,  float width, float height, float depth);
void destroySnakeSkeleton(SnakeSkeleton skeleton);

void createSnakeBones
(ecs_entity_t entity, 
uint32_t numberSections, 
float space,  float width, float height, float depth);
void destroySnakeBones(ecs_entity_t entity);
void destroySnakeBones2(ecs_entity_t entity);

void createSnakeAnimation1
(ecs_entity_t entity, ecs_entity_t animationEntity,
uint32_t numberSections,
float space, float depth,
float bending, float scratching, float radius, float timeStepSize,
uint32_t numberKeyFrames, uint32_t boneLevel);
void destroySnakeAnimation1_1
(ecs_entity_t entity, ecs_entity_t animationEntity);
void destroySnakeAnimation1_2
(ecs_entity_t entity, ecs_entity_t animationEntity);

#endif