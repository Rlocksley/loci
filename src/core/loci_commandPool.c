#include "loci_commandPool.h"

VkCommandPool loci_vkCommandPool;

void loci_createCommandPool()
{
    VkCommandPoolCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    createInfo.queueFamilyIndex = loci_queueFamilyIndex;
    createInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreateCommandPool
    (loci_vkDevice,
    &createInfo,
    NULL,
    &loci_vkCommandPool),
    "createCommandPool",
    "vkCreateCommandPool")

    #ifdef LOCI_DEBUG
    LOCI_LOGI("VkCommandPool", "created", "")
    #endif
}

void loci_destroyCommandPool()
{
    vkDestroyCommandPool
    (loci_vkDevice,
    loci_vkCommandPool,
    NULL);
}