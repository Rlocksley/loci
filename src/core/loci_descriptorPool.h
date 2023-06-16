#ifndef LOCI_DESCRIPTORPOOL_H
#define LOCI_DESCRIPTORPOOL_H

#include "loci_device.h"

uint32_t loci_maxNumberUniformBuffers;
uint32_t loci_maxNumberStorageBuffers;
uint32_t loci_maxNumberCombinedImageSamplers;
uint32_t loci_maxNumberStorageImages; 
uint32_t loci_maxNumberTopAccelerations;
uint32_t loci_maxNumberDescriptorSets;
    
VkDescriptorPool loci_vkDescriptorPool;

void loci_createDescriptorPool();
void loci_destroyDescriptorPool();

#endif