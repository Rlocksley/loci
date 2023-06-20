#ifndef LOCI_CHANNEL_H
#define LOCI_CHANNEL_H

#include "../core/loci_global.h"
#include "../core/loci_world.h"


typedef struct Loci_KeyFrameScale
{
    vec3 scale;
    float time;
}Loci_KeyFrameScale;

typedef struct Loci_KeyFrameRotation
{
    versor rotation;
    float time;
}Loci_KeyFrameRotation;

typedef struct Loci_KeyFramePosition
{
    vec3 position;
    float time;
}Loci_KeyFramePosition;


typedef struct Loci_Channel
{
    uint32_t numberScales;
    uint32_t numberRotations;
    uint32_t numberPositions;
    Loci_KeyFrameScale* pScales;
    Loci_KeyFrameRotation* pRotations;
    Loci_KeyFramePosition* pPositions;
}Loci_Channel;

void loci_createChannel(ecs_entity_t entity, ecs_entity_t animationEntity,
Loci_KeyFrameScale* pScales, uint32_t numberScales,
Loci_KeyFrameRotation* pRotations, uint32_t numberRotations,
Loci_KeyFramePosition* pPositions ,uint32_t numberPositions);
void loci_destroyChannel(ecs_entity_t entity, ecs_entity_t animationEntity);

#endif