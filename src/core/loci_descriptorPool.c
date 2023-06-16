#include "loci_descriptorPool.h"

uint32_t loci_maxNumberUniformBuffers;
uint32_t loci_maxNumberStorageBuffers;
uint32_t loci_maxNumberCombinedImageSamplers;
uint32_t loci_maxNumberStorageImages; 
uint32_t loci_maxNumberTopAccelerations;
uint32_t loci_maxNumberDescriptorSets;
VkDescriptorPool loci_vkDescriptorPool;

void loci_createDescriptorPool()
{
    uint32_t count = 0;
    VkDescriptorPoolSize sizes[6];

    if(loci_maxNumberUniformBuffers > 0)
    {
        sizes[count].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        sizes[count].descriptorCount = loci_maxNumberUniformBuffers;
        count++;
    }
    if(loci_maxNumberStorageBuffers > 0)
    {
        sizes[count].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        sizes[count].descriptorCount = loci_maxNumberStorageBuffers;
        count++;
    }
    if(loci_maxNumberCombinedImageSamplers > 0)
    {
        sizes[count].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        sizes[count].descriptorCount = loci_maxNumberCombinedImageSamplers;
        count++;
    }
    if(loci_maxNumberStorageImages > 0)
    {
        sizes[count].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        sizes[count].descriptorCount = loci_maxNumberStorageImages;
        count++;
    }
    if(loci_maxNumberTopAccelerations > 0)
    {
        sizes[count].type = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
        sizes[count].descriptorCount = loci_maxNumberTopAccelerations;
        count++;
    }

    VkDescriptorPoolCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    createInfo.poolSizeCount = count;
    createInfo.pPoolSizes = sizes;
    createInfo.maxSets = loci_maxNumberDescriptorSets;
    createInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreateDescriptorPool
    (loci_vkDevice,
    &createInfo,
    NULL,
    &loci_vkDescriptorPool),
    "createDescriptorPool",
    "vkCreateDescriptorPool")

    #ifdef LOCI_DEBUG
    LOCI_LOGI("VkDescriptorPool", "created", "")
    #endif
}

void loci_destroyDescriptorPool()
{
    vkDestroyDescriptorPool
    (loci_vkDevice,
    loci_vkDescriptorPool,
    NULL);
}