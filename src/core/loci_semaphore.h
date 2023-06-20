#ifndef LOCI_SEMAPHORE_H
#define LOCI_SEMAPHORE_H

#include "loci_device.h"

extern VkSemaphore loci_imageIndexVkSemaphore;
extern VkSemaphore loci_skeletonUpdateVkSemaphore;
extern VkSemaphore loci_bottomAccelerationUpdateVkSemaphore;
extern VkSemaphore loci_topAccelerationUpdateVkSemaphore;
extern VkSemaphore loci_drawVkSemaphore;

void loci_createSemaphore(VkSemaphore* pSemaphore);
void loci_destroySemaphore(VkSemaphore semaphore);

#endif