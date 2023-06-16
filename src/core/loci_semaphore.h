#ifndef LOCI_SEMAPHORE_H
#define LOCI_SEMAPHORE_H

#include "loci_device.h"

VkSemaphore loci_imageIndexVkSemaphore;
VkSemaphore loci_animationUpdateVkSemaphore;
VkSemaphore loci_bottomAccelerationUpdateVkSemaphore;
VkSemaphore loci_topAccelerationUpdateVkSemaphore;
VkSemaphore loci_drawVkSemaphore;

void loci_createSemaphore(VkSemaphore* pSemaphore);
void loci_destroySemaphore(VkSemaphore semaphore);

#endif