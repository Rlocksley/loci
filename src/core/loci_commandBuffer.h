#ifndef LOCI_COMMANDBUFFER_H
#define LOCI_COMMANDBUFFER_H

#include "loci_commandPool.h"

extern VkCommandBuffer loci_buildVkCommandBuffer;
extern VkCommandBuffer loci_skeletonUpdateVkCommandBuffer;
extern VkCommandBuffer loci_bottomAccelerationUpdateVkCommandBuffer;
extern VkCommandBuffer loci_topAccelerationUpdateVkCommandBuffer;
extern VkCommandBuffer loci_drawVkCommandBuffer;

void loci_createCommandBuffer(VkCommandBuffer* pCommandBuffer);
void loci_destroyCommandBuffer(VkCommandBuffer* pCommandBuffer);

void loci_beginCommand(VkCommandBuffer commandBuffer);
void loci_endCommand(VkCommandBuffer commandBuffer);

#endif