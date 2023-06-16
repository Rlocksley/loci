#ifndef LOCI_IMAGE_H
#define LOCI_IMAGE_H

#include "loci_device.h"
#include "loci_commandBuffer.h"
#include "loci_queue.h"
#include "loci_fence.h"
#include "loci_buffer.h"

typedef struct Loci_RayImage
{
    VkImage vkImage;
    VkImageView vkImageView;
    VkDeviceMemory vkDeviceMemory;
}Loci_RayImage;

extern Loci_RayImage loci_rayImage;


void loci_createRayImage();
void loci_destroyRayImage();




typedef struct Loci_Image
{
    VkImage vkImage;
    VkImageView vkImageView;
    VkSampler vkSampler;
    VkDeviceMemory vkDeviceMemory;
    uint32_t index;
}Loci_Image;

Loci_Image loci_createImage
(const char* filePath);
void loci_destroyImage(Loci_Image image);


#endif