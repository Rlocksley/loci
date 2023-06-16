#include "loci_queue.h"

void loci_getQueue()
{
    vkGetDeviceQueue
    (loci_vkDevice, 
    loci_queueFamilyIndex,
    0, &loci_vkQueue);

    #ifdef LOCI_DEBUG
    LOCI_LOGI("VkQueue", "got", "")
    #endif
}

void loci_submitSingleCommand
(VkCommandBuffer commandBuffer, VkFence fence)
{
    VkPipelineStageFlags stageFlags[1] = 
    {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT};

    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = stageFlags;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = NULL;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = NULL;
    submitInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkQueueSubmit
    (loci_vkQueue,
    1, &submitInfo,
    fence),
    "submitSingleCommand",
    "vkQueueSubmit")

    LOCI_CHECK_VULKAN
    (vkWaitForFences
    (loci_vkDevice,1,
    &fence, 
    VK_TRUE, UINT64_MAX),
    "submitSingleCommand",
    "vkQueueSubmit")

    LOCI_CHECK_VULKAN
    (vkResetFences
    (loci_vkDevice,1, &fence),
    "submitSingleCommand",
    "vkResetFences")
}

void loci_submitCommand
(VkPipelineStageFlags stageFlags, VkSemaphore waitSemaphore,
VkCommandBuffer commandBuffer, VkSemaphore signalSemaphore,
VkFence fence)
{
    VkSubmitInfo submitInfo;
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = &stageFlags;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &waitSemaphore;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &signalSemaphore;
    submitInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkQueueSubmit
    (loci_vkQueue,
    1, &submitInfo,
    fence),
    "submitCommand",
    "vkQueueSubmit")
}

void loci_presentCommand(VkSemaphore waitSemaphore)
{
    VkPresentInfoKHR presentInfo;
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &waitSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &loci_swapchain.vkSwapchainKHR;
    presentInfo.pImageIndices = &loci_swapchain.imageIndex;
    presentInfo.pResults = NULL;
    presentInfo.pNext = NULL;
    
    LOCI_CHECK_VULKAN
    (vkQueuePresentKHR
    (loci_vkQueue,
    &presentInfo),
    "presentCommand",
    "vkQueuePresentKHR")
}
