#include "loci_commandBuffer.h"


VkCommandBuffer loci_buildVkCommandBuffer;
VkCommandBuffer loci_animationUpdateVkCommandBuffer;
VkCommandBuffer loci_bottomAccelerationUpdateVkCommandBuffer;
VkCommandBuffer loci_topAccelerationUpdateVkCommandBuffer;
VkCommandBuffer loci_drawVkCommandBuffer;


void loci_createCommandBuffer(VkCommandBuffer* pCommandBuffer)
{
    VkCommandBufferAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = loci_vkCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;
    allocInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkAllocateCommandBuffers
    (loci_vkDevice,
    &allocInfo,
    pCommandBuffer),
    "createCommandBuffer",
    "vkAllocateCommandBuffers")

    #ifdef LOCI_DEBUG
    LOCI_LOGI("VkCommandBuffer", "created", "")
    #endif
}

void loci_destroyCommandBuffer(VkCommandBuffer* pCommandBuffer)
{
    vkFreeCommandBuffers
    (loci_vkDevice,
    loci_vkCommandPool,
    1, pCommandBuffer);
}

void loci_beginCommand(VkCommandBuffer commandBuffer)
{
    VkCommandBufferBeginInfo beginInfo;
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    beginInfo.pInheritanceInfo = NULL;
    beginInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkBeginCommandBuffer
    (commandBuffer,
    &beginInfo),
    "beginCommand",
    "vkBeginCommandBuffer")
}

void loci_endCommand(VkCommandBuffer commandBuffer)
{
    LOCI_CHECK_VULKAN
    (vkEndCommandBuffer
    (commandBuffer),
    "endCommand",
    "vkEndCommandBuffer")
}