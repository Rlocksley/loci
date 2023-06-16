#include "loci_semaphore.h"


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