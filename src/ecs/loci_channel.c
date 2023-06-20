#include "loci_channel.h"

void loci_createChannel(ecs_entity_t entity, ecs_entity_t animationEntity,
Loci_KeyFrameScale* pScales, uint32_t numberScales,
Loci_KeyFrameRotation* pRotations, uint32_t numberRotations,
Loci_KeyFramePosition* pPositions ,uint32_t numberPositions)
{
    Loci_Channel channel;
    channel.numberScales = numberScales;
    channel.numberRotations = numberRotations;
    channel.numberPositions = numberPositions;
    channel.pScales = pScales;
    channel.pRotations = pRotations;
    channel.pPositions = pPositions;

    loci_setPairSecond(entity, animationEntity, Loci_Channel, channel)
}

void loci_destroyChannel(ecs_entity_t entity, ecs_entity_t animationEntity)
{
    const Loci_Channel* pChannel;
    loci_getPairSecond(entity, animationEntity, Loci_Channel, pChannel);
    Loci_Channel channel = *pChannel;

    free(channel.pPositions);
    free(channel.pRotations);
    free(channel.pScales);

    ECS_COMPONENT(loci_pWorld, Loci_Channel);
    loci_removePair(entity, animationEntity, ecs_id(Loci_Channel))
}
