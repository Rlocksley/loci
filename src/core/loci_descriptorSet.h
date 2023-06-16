#ifndef LOCI_DESCRIPTORSET_H
#define LOCI_DESCRIPTORSET_H

#include "loci_descriptorPool.h"
#include "loci_acceleration.h"
#include "loci_image.h"
#include "loci_eye.h"

typedef struct Loci_RayDescriptorSet
{
    VkDescriptorSetLayout vkDescriptorSetLayout;
    VkDescriptorSet vkDescriptorSet;
}Loci_RayDescriptorSet;

Loci_RayDescriptorSet loci_createRayDescriptorSet
(uint32_t maxNumberImages);
void loci_destroyRayDescriptorSet(Loci_RayDescriptorSet descriptorSet);
void loci_updateRayDescriptorSet
(Loci_RayDescriptorSet descriptorSet,
Loci_Image* images, uint32_t numberImages);
\
Loci_RayDescriptorSet loci_updateRayDescriptorSet2
(Loci_RayDescriptorSet descriptorSet,
Loci_TopAcceleration acceleration,
Loci_BufferInterface objectBuffer, uint32_t numberObjects,
Loci_Image images[], uint32_t numberImages,
Loci_BufferInterface pointLightBuffer);

#endif