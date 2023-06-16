#ifndef LOCI_DEVICE_H
#define LOCI_DEVICE_H
#include "loci_device.h"
#endif
#ifndef LOCI_SEMAPHORE_H
#define LOCI_SEMAPHORE_H
#include "loci_semaphore.h"
#endif

typedef struct Loci_Swapchain
{
    VkSwapchainKHR vkSwapchainKHR;
    uint32_t numberImages;
    VkImage* pVkImages;
    VkImageView* pVkImageViews;
    uint32_t imageIndex;
}Loci_Swapchain;

Loci_Swapchain loci_swapchain;

void loci_createSwapchain();
void loci_destroySwapchain();
void loci_getSwapchainImageIndex();

void loci_createSwapchain()
{
    const uint32_t minImageCount = 
    loci_vkSurfaceCapabilitiesKHR.maxImageCount > 0 ?
    loci_vkSurfaceCapabilitiesKHR.maxImageCount :
    loci_vkSurfaceCapabilitiesKHR.minImageCount + 1;

    VkSwapchainCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.flags = 0;
    createInfo.surface = loci_vkSurfaceKHR;
    createInfo.minImageCount = minImageCount;
    createInfo.imageFormat = loci_vkSurfaceFormatKHR.format;
    createInfo.imageColorSpace = loci_vkSurfaceFormatKHR.colorSpace;
    createInfo.imageExtent = loci_vkExtent2D;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.preTransform = loci_vkSurfaceCapabilitiesKHR.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = loci_vkPresentModeKHR;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = NULL;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = NULL;
    createInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreateSwapchainKHR
    (loci_vkDevice,
    &createInfo,
    NULL,
    &loci_swapchain.vkSwapchainKHR),
    "createSwapchain",
    "vkCreateSwapchainKHR")


    LOCI_CHECK_VULKAN
    (vkGetSwapchainImagesKHR
    (loci_vkDevice,
    loci_swapchain.vkSwapchainKHR,
    &loci_swapchain.numberImages, NULL),
    "createSwapchain",
    "vkGetSwapchainImagesKHR")

    if((loci_swapchain.pVkImages = 
    (VkImage*) malloc(sizeof(VkImage)*loci_swapchain.numberImages)) == NULL)
    {
        LOCI_LOGE("createSwapchain", "malloc() for pVkImages", "failed")
    }

    LOCI_CHECK_VULKAN
    (vkGetSwapchainImagesKHR
    (loci_vkDevice,
    loci_swapchain.vkSwapchainKHR,
    &loci_swapchain.numberImages, 
    loci_swapchain.pVkImages),
    "createSwapchain",
    "vkGetSwapchainImagesKHR")

    if((loci_swapchain.pVkImageViews = 
    (VkImageView*) malloc(sizeof(VkImageView)*loci_swapchain.numberImages)) == NULL)
    {
        LOCI_LOGE("createSwapchain", "malloc() for pVkImageViews", "failed")
    }

    for(uint32_t i = 0; i < loci_swapchain.numberImages; i++)
    {
        VkImageViewCreateInfo ivCreateInfo;
        ivCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        ivCreateInfo.flags = 0;
        ivCreateInfo.image = loci_swapchain.pVkImages[i];
        ivCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        ivCreateInfo.format = loci_vkSurfaceFormatKHR.format;
        ivCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        ivCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        ivCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        ivCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        ivCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        ivCreateInfo.subresourceRange.baseMipLevel = 0;
        ivCreateInfo.subresourceRange.levelCount = 1;
        ivCreateInfo.subresourceRange.baseArrayLayer = 0;
        ivCreateInfo.subresourceRange.layerCount = 1;
        ivCreateInfo.pNext = NULL;

        LOCI_CHECK_VULKAN
        (vkCreateImageView
        (loci_vkDevice,
        &ivCreateInfo,
        NULL,
        &loci_swapchain.pVkImageViews[i]),
        "createSwapchain",
        "vkCreateImageView")
    }

    #ifdef LOCI_DEBUG
    LOCI_LOGI("Swapchain", "created", "")
    #endif
}

void loci_destroySwapchain()
{
    for(uint32_t i = 0; i < loci_swapchain.numberImages; i++)
    {
        vkDestroyImageView
        (loci_vkDevice,
        loci_swapchain.pVkImageViews[i],
        NULL);
    }

    free(loci_swapchain.pVkImageViews);

    free(loci_swapchain.pVkImages);

    vkDestroySwapchainKHR
    (loci_vkDevice,
    loci_swapchain.vkSwapchainKHR,
    NULL);
}

void loci_getSwapchainImageIndex()
{
    LOCI_CHECK_VULKAN
    (vkAcquireNextImageKHR
    (loci_vkDevice,
    loci_swapchain.vkSwapchainKHR,
    UINT64_MAX,
    loci_imageIndexVkSemaphore,
    VK_NULL_HANDLE,
    &loci_swapchain.imageIndex),
    "getSwapchainImageIndex",
    "vkAcquireNextImageKHR")
}