#ifndef LOCI_DEVICE_H
#define LOCI_DEVICE_H
#include "loci_device.h"
#endif

VkFence loci_buildVkFence;
VkFence loci_drawVkFence;

void loci_createFence(VkFence* pFence, VkFenceCreateFlags flags);
void loci_destroyFence(VkFence fence);
void loci_waitForFence(VkFence* pFence);



void loci_createFence(VkFence* pFence, VkFenceCreateFlags flags)
{
    VkFenceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.flags = flags;
    createInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreateFence
    (loci_vkDevice,
    &createInfo, NULL,
    pFence),
    "createFence",
    "vkCreateFence")

    #ifdef LOCI_DEBUG
    LOCI_LOGI("VkFence", "created", "")
    #endif
}

void loci_destroyFence(VkFence fence)
{
    vkDestroyFence
    (loci_vkDevice,
    fence, NULL);
}

void loci_waitForFence(VkFence* pFence)
{
    LOCI_CHECK_VULKAN
    (vkWaitForFences
    (loci_vkDevice,
    1, pFence,
    VK_TRUE, UINT64_MAX),
    "waitForFence",
    "vkWaitForFences")

    LOCI_CHECK_VULKAN
    (vkResetFences
    (loci_vkDevice,
    1, pFence),
    "waitForFence",
    "vkResetFences")
}