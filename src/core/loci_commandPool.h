#ifndef LOCI_COMMANDPOOL_H
#define LOCI_COMMANDPOOL_h

#include "loci_device.h"

VkCommandPool loci_vkCommandPool;

void loci_createCommandPool();
void loci_destroyCommandPool();


#endif