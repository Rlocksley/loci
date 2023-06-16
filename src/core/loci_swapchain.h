#ifndef LOCI_SWAPCHAIN_H
#define LOCI_SWAPCHAIN_H

#include "loci_device.h"
#include "loci_semaphore.h"

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

#endif