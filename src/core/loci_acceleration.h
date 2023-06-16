#ifndef LOCI_ACCELERATION_H
#define LOCI_ACCELERATION_H

#include "loci_queue.h"
#include "loci_buffer.h"
#include "loci_commandBuffer.h"
#include "loci_fence.h"

typedef struct Loci_TopAcceleration
{
    VkAccelerationStructureKHR vkAccelerationStructureKHR;
    uint64_t deviceAddress;
    Loci_Buffer buffer;
    Loci_Buffer scratchBuffer;
}Loci_TopAcceleration;

Loci_TopAcceleration loci_createTopAcceleration
(Loci_BufferInterface instanceBuffer, uint32_t numberInstances);
void loci_destroyTopAcceleration(Loci_TopAcceleration acceleration);
void loci_updateTopAcceleration
(Loci_TopAcceleration acceleration,
Loci_BufferInterface instanceBuffer, 
uint32_t numberInstances,
bool animationUpdate, bool bottomAccelerationUpdate);


typedef struct Loci_BottomAcceleration
{
    VkAccelerationStructureKHR vkAccelerationStructureKHR;
    uint64_t deviceAddress;
    Loci_Buffer buffer;
    Loci_Buffer scratchBuffer;
}Loci_BottomAcceleration;

Loci_BottomAcceleration loci_createBottomAcceleration
(Loci_Buffer vertexBuffer, Loci_Buffer indexBuffer,
Loci_Buffer transformBuffer);
void loci_destroyBottomAcceleration
(Loci_BottomAcceleration acceleration);

#endif