#ifndef LOCI_QUEUE_H
#define LOCI_QUEUE_H

#include "loci_device.h"
#include "loci_swapchain.h"

VkQueue loci_vkQueue;

void loci_getQueue();

void loci_submitSingleCommand
(VkCommandBuffer commandBuffer, VkFence fence);
void loci_submitCommand
(VkPipelineStageFlags stageFlags, VkSemaphore waitSemaphore,
VkCommandBuffer commandBuffer, VkSemaphore signalSemaphore,
VkFence fence);
void loci_presentCommand(VkSemaphore waitSemaphore);

#endif