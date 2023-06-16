#ifndef LOCI_DESCRIPTORPOOL_H
#define LOCI_DESCRIPTORPOOL_H

#include "loci_device.h"

extern uint32_t loci_maxNumberUniformBuffers;
extern uint32_t loci_maxNumberStorageBuffers;
extern uint32_t loci_maxNumberCombinedImageSamplers;
extern uint32_t loci_maxNumberStorageImages; 
extern uint32_t loci_maxNumberTopAccelerations;
extern uint32_t loci_maxNumberDescriptorSets;
    
extern VkDescriptorPool loci_vkDescriptorPool;

void loci_createDescriptorPool();
void loci_destroyDescriptorPool();

#endif