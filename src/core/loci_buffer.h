#ifndef LOCI_BUFFER_H
#define LOCI_BUFFER_H

#include "loci_device.h"

typedef struct Loci_Buffer
{
   uint32_t numberElements;
   uint32_t elementSize;
   VkBuffer vkBuffer;
   VkDeviceMemory vkDeviceMemory;
   uint64_t deviceAddress;
}Loci_Buffer;

typedef struct Loci_BufferInterface
{
   uint32_t numberElements;
   uint32_t elementSize;
   VkBuffer vkBuffer;
   VkDeviceMemory vkDeviceMemory;
   uint64_t deviceAddress;
   void* pMemory;
}Loci_BufferInterface;

Loci_Buffer loci_createBuffer
(uint32_t numberElements, uint32_t elementSize, 
VkBufferUsageFlags usage);
void loci_destroyBuffer(Loci_Buffer buffer);
Loci_BufferInterface loci_createBufferInterface
(uint32_t numberElements, uint32_t elementSize, 
VkBufferUsageFlags usage);
void loci_destroyBufferInterface(Loci_BufferInterface buffer);

#endif