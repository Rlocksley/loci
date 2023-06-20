#include "loci_animation.h"




void loci_createAnimation
(ecs_entity_t entity, 
ecs_entity_t animationEntity, float time)
{
    Loci_Animation animation;
    animation.entity = entity;
    animation.animation = animationEntity;
    animation.time = time;

    loci_set(entity, Loci_Animation, animation)
}


void loci_destroyAnimation(ecs_entity_t entity)
{
    loci_remove(entity, Loci_Animation);
}
