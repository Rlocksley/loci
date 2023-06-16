#ifndef LOCI_FENCE_H
#define LOCI_FENCE_H

#include "loci_device.h"

VkFence loci_buildVkFence;
VkFence loci_drawVkFence;

void loci_createFence(VkFence* pFence, VkFenceCreateFlags flags);
void loci_destroyFence(VkFence fence);
void loci_waitForFence(VkFence* pFence);

#endif