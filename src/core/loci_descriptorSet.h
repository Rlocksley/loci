#ifndef LOCI_DESCRIPTORPOOL_H
#define LOCI_DESCRIPTORPOOL_H
#include "loci_descriptorPool.h"
#endif
#ifndef LOCI_ACCELERATION_H
#define LOCI_ACCELERATION_H
#include "loci_acceleration.h"
#endif
#ifndef LOCI_IMAGE_H
#define LOCI_IMAGE_H
#include "loci_image.h"
#endif
#ifndef LOCI_EYE_H
#define LOCI_EYE_H
#include "loci_eye.h"
#endif

typedef struct Loci_RayDescriptorSet
{
    VkDescriptorSetLayout vkDescriptorSetLayout;
    VkDescriptorSet vkDescriptorSet;
}Loci_RayDescriptorSet;

Loci_RayDescriptorSet loci_rayDescriptorSet;

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


Loci_RayDescriptorSet loci_createRayDescriptorSet
(uint32_t maxNumberImages)
{
    Loci_RayDescriptorSet descriptorSet;

    VkDescriptorSetLayoutBinding bindings[6];
    bindings[0].binding = 0;
    bindings[0].descriptorCount = 1;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    bindings[0].stageFlags = VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
    bindings[0].pImmutableSamplers = NULL;
    bindings[1].binding = 1;
    bindings[1].descriptorCount = 1;
    bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    bindings[1].stageFlags = VK_SHADER_STAGE_RAYGEN_BIT_KHR;
    bindings[1].pImmutableSamplers = NULL;
    bindings[2].binding = 2;
    bindings[2].descriptorCount = 1;
    bindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[2].stageFlags = VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
    bindings[2].pImmutableSamplers = NULL;
    bindings[3].binding = 3;
    bindings[3].descriptorCount = 1;
    bindings[3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    bindings[3].stageFlags = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
    bindings[3].pImmutableSamplers = NULL;
    bindings[4].binding = 4;
    bindings[4].descriptorCount = 1;
    bindings[4].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[4].stageFlags = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
    bindings[4].pImmutableSamplers = NULL;
    bindings[5].binding = 5;
    bindings[5].descriptorCount = maxNumberImages;
    bindings[5].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    bindings[5].stageFlags = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
    bindings[5].pImmutableSamplers = NULL;

    VkDescriptorBindingFlags layoutFlags[6] = 
    {0,0,0,0,0, VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT};

    VkDescriptorSetLayoutBindingFlagsCreateInfo layoutFlagsCreateInfo;
    layoutFlagsCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
    layoutFlagsCreateInfo.bindingCount = 6;
    layoutFlagsCreateInfo.pBindingFlags = layoutFlags;
    layoutFlagsCreateInfo.pNext = NULL;

    VkDescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.flags = 0;
    layoutInfo.bindingCount = 6;
    layoutInfo.pBindings = bindings;
    layoutInfo.pNext = &layoutFlagsCreateInfo;

    LOCI_CHECK_VULKAN
    (vkCreateDescriptorSetLayout
    (loci_vkDevice,
    &layoutInfo, NULL,
    &descriptorSet.vkDescriptorSetLayout),
    "createRayDescriptorSet",
    "vkDescriptorSetLayout")

    VkDescriptorSetVariableDescriptorCountAllocateInfo countAllocInfo;
    countAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
    countAllocInfo.descriptorSetCount = 1;
    countAllocInfo.pDescriptorCounts = &maxNumberImages;
    countAllocInfo.pNext = NULL;

    VkDescriptorSetAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = loci_vkDescriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSet.vkDescriptorSetLayout;
    allocInfo.pNext = &countAllocInfo;

    LOCI_CHECK_VULKAN
    (vkAllocateDescriptorSets
    (loci_vkDevice,
    &allocInfo,
    &descriptorSet.vkDescriptorSet),
    "createRayDescriptorSet",
    "vkAllocateDescriptorSets")

    return descriptorSet;
}

void loci_destroyRayDescriptorSet(Loci_RayDescriptorSet descriptorSet)
{
    vkFreeDescriptorSets
    (loci_vkDevice,
    loci_vkDescriptorPool,
    1, &descriptorSet.vkDescriptorSet);

    vkDestroyDescriptorSetLayout
    (loci_vkDevice,
    descriptorSet.vkDescriptorSetLayout,
    NULL);
}

void loci_updateRayDescriptorSet
(Loci_RayDescriptorSet descriptorSet,
Loci_Image* images, uint32_t numberImages)
{
    VkDescriptorImageInfo textureInfos[numberImages];
    for(uint32_t i = 0; i < numberImages; i++)
    {
        textureInfos[i].imageView = images[i].vkImageView;
        textureInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        textureInfos[i].sampler = images[i].vkSampler;
    }
    VkWriteDescriptorSet imageWriteInfo1;
    imageWriteInfo1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    imageWriteInfo1.dstSet = descriptorSet.vkDescriptorSet;
    imageWriteInfo1.dstBinding = 5;
    imageWriteInfo1.descriptorCount = numberImages;
    imageWriteInfo1.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    imageWriteInfo1.pImageInfo = textureInfos;
    imageWriteInfo1.dstArrayElement = 0;
    imageWriteInfo1.pBufferInfo = NULL;
    imageWriteInfo1.pTexelBufferView = NULL;
    imageWriteInfo1.pNext = NULL;

    vkUpdateDescriptorSets
    (loci_vkDevice, 
    1, &imageWriteInfo1, 
    0, VK_NULL_HANDLE);
}

Loci_RayDescriptorSet loci_updateRayDescriptorSet2
(Loci_RayDescriptorSet descriptorSet,
Loci_TopAcceleration acceleration,
Loci_BufferInterface objectBuffer, uint32_t numberObjects,
Loci_Image images[], uint32_t numberImages,
Loci_BufferInterface pointLightBuffer)
{
    vkFreeDescriptorSets
    (loci_vkDevice,
    loci_vkDescriptorPool,
    1, &descriptorSet.vkDescriptorSet);


    VkDescriptorSetVariableDescriptorCountAllocateInfo countAllocInfo;
    countAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
    countAllocInfo.descriptorSetCount = 1;
    countAllocInfo.pDescriptorCounts = &numberImages;
    countAllocInfo.pNext = NULL;

    VkDescriptorSetAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = loci_vkDescriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &descriptorSet.vkDescriptorSetLayout;
    allocInfo.pNext = &countAllocInfo;

    LOCI_CHECK_VULKAN
    (vkAllocateDescriptorSets
    (loci_vkDevice,
    &allocInfo,
    &descriptorSet.vkDescriptorSet),
    "createRayDescriptorSet",
    "vkAllocateDescriptorSets")

    VkWriteDescriptorSetAccelerationStructureKHR accInfo;
    accInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
    accInfo.accelerationStructureCount = 1;
    accInfo.pAccelerationStructures = &acceleration.vkAccelerationStructureKHR;
    accInfo.pNext = NULL;

    VkWriteDescriptorSet accWriteInfo;
    accWriteInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    accWriteInfo.dstSet = descriptorSet.vkDescriptorSet;
    accWriteInfo.dstBinding = 0;
    accWriteInfo.descriptorCount = 1;
    accWriteInfo.descriptorType = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    accWriteInfo.pNext = &accInfo;

    VkDescriptorImageInfo imageInfo;
    imageInfo.imageView = loci_rayImage.vkImageView;
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    imageInfo.sampler = NULL;

    VkWriteDescriptorSet imageWriteInfo0;
    imageWriteInfo0.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    imageWriteInfo0.dstSet = descriptorSet.vkDescriptorSet;
    imageWriteInfo0.dstBinding = 1;
    imageWriteInfo0.descriptorCount = 1;
    imageWriteInfo0.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    imageWriteInfo0.pImageInfo = &imageInfo;
    imageWriteInfo0.dstArrayElement = 0;
    imageWriteInfo0.pBufferInfo = NULL;
    imageWriteInfo0.pTexelBufferView  = NULL;
    imageWriteInfo0.pNext = NULL;

    VkDescriptorBufferInfo uniBufferInfo;
    uniBufferInfo.buffer = loci_eyeBuffer.vkBuffer;
    uniBufferInfo.offset = 0;
    uniBufferInfo.range = sizeof(Loci_Eye);

    VkWriteDescriptorSet uniBufferWriteInfo;
    uniBufferWriteInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    uniBufferWriteInfo.dstSet = descriptorSet.vkDescriptorSet;
    uniBufferWriteInfo.dstBinding = 2;
    uniBufferWriteInfo.descriptorCount = 1;
    uniBufferWriteInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uniBufferWriteInfo.pBufferInfo = &uniBufferInfo;
    uniBufferWriteInfo.dstArrayElement = 0;
    uniBufferWriteInfo.pImageInfo = NULL;
    uniBufferWriteInfo.pTexelBufferView = NULL;
    uniBufferWriteInfo.pNext = NULL;

    VkDescriptorBufferInfo storageBufferInfo;
    storageBufferInfo.buffer = objectBuffer.vkBuffer;
    storageBufferInfo.offset = 0;
    storageBufferInfo.range = numberObjects * objectBuffer.elementSize;

    VkWriteDescriptorSet storageBufferWriteInfo;
    storageBufferWriteInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    storageBufferWriteInfo.dstSet = descriptorSet.vkDescriptorSet;
    storageBufferWriteInfo.dstBinding = 3;
    storageBufferWriteInfo.descriptorCount = 1;
    storageBufferWriteInfo.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    storageBufferWriteInfo.pBufferInfo = &storageBufferInfo;
    storageBufferWriteInfo.dstArrayElement = 0;
    storageBufferWriteInfo.pImageInfo = NULL;
    storageBufferWriteInfo.pTexelBufferView = NULL;
    storageBufferWriteInfo.pNext = NULL;

    VkDescriptorBufferInfo pointLightBufferInfo;
    pointLightBufferInfo.buffer = pointLightBuffer.vkBuffer;
    pointLightBufferInfo.offset = 0;
    pointLightBufferInfo.range = pointLightBuffer.numberElements * pointLightBuffer.elementSize;

    VkWriteDescriptorSet pointLightBufferWriteInfo;
    pointLightBufferWriteInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    pointLightBufferWriteInfo.dstSet = descriptorSet.vkDescriptorSet;
    pointLightBufferWriteInfo.dstBinding = 4;
    pointLightBufferWriteInfo.descriptorCount = 1;
    pointLightBufferWriteInfo.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    pointLightBufferWriteInfo.pBufferInfo = &pointLightBufferInfo;
    pointLightBufferWriteInfo.dstArrayElement = 0;
    pointLightBufferWriteInfo.pImageInfo = NULL;
    pointLightBufferWriteInfo.pTexelBufferView = NULL;
    pointLightBufferWriteInfo.pNext = NULL;

    VkDescriptorImageInfo textureInfos[numberImages];
    for(uint32_t i = 0; i < numberImages; i++)
    {
        textureInfos[i].imageView = images[i].vkImageView;
        textureInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        textureInfos[i].sampler = images[i].vkSampler;
    }
    VkWriteDescriptorSet imageWriteInfo1;
    imageWriteInfo1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    imageWriteInfo1.dstSet = descriptorSet.vkDescriptorSet;
    imageWriteInfo1.dstBinding = 5;
    imageWriteInfo1.descriptorCount = numberImages;
    imageWriteInfo1.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    imageWriteInfo1.pImageInfo = textureInfos;
    imageWriteInfo1.dstArrayElement = 0;
    imageWriteInfo1.pBufferInfo = NULL;
    imageWriteInfo1.pTexelBufferView = NULL;
    imageWriteInfo1.pNext = NULL;

    VkWriteDescriptorSet writeDescrSets[6] = 
    {
        accWriteInfo,
        imageWriteInfo0,
        uniBufferWriteInfo,
        storageBufferWriteInfo,
        pointLightBufferWriteInfo,
        imageWriteInfo1
    };

    vkUpdateDescriptorSets
    (loci_vkDevice, 6, writeDescrSets,
    0, VK_NULL_HANDLE);

    return descriptorSet;
}
