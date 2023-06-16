#ifndef LOCI_DEVICE_H
#define LOCI_DEVICE_H
#include "loci_device.h"
#endif

VkSemaphore loci_imageIndexVkSemaphore;
VkSemaphore loci_animationUpdateVkSemaphore;
VkSemaphore loci_bottomAccelerationUpdateVkSemaphore;
VkSemaphore loci_topAccelerationUpdateVkSemaphore;
VkSemaphore loci_drawVkSemaphore;

void loci_createSemaphore(VkSemaphore* pSemaphore);
void loci_destroySemaphore(VkSemaphore semaphore);


void loci_createSemaphore(VkSemaphore* pSemaphore)
{
    VkSemaphoreCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreateSemaphore
    (loci_vkDevice,
    &createInfo, NULL,
    pSemaphore),
    "createSemaphore",
    "vkCreateSemaphore")

    #ifdef LOCI_DEBUG
    LOCI_LOGI("VkSemaphore", "created", "")
    #endif
}

void loci_destroySemaphore(VkSemaphore semaphore)
{
    vkDestroySemaphore
    (loci_vkDevice,
    semaphore, 
    NULL);
}