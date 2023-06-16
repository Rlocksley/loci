#include "loci_buffer.h"

Loci_Buffer loci_createBuffer
(uint32_t numberElements, uint32_t elementSize, 
VkBufferUsageFlags usage)
{
   Loci_Buffer buffer;
   buffer.numberElements = numberElements;
   buffer.elementSize = elementSize;

   VkBufferCreateInfo createInfo;
   createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
   createInfo.flags = 0;
   createInfo.size = numberElements * elementSize;
   createInfo.usage = usage | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT; 
   createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
   createInfo.queueFamilyIndexCount = 0;
   createInfo.pQueueFamilyIndices = NULL;
   createInfo.pNext = NULL;

   LOCI_CHECK_VULKAN
   (vkCreateBuffer
   (loci_vkDevice,
   &createInfo, 
   NULL,
   &buffer.vkBuffer),
   "createBuffer",
   "vkCreateBuffer")

   VkMemoryRequirements memRequ;
   vkGetBufferMemoryRequirements
   (loci_vkDevice,
   buffer.vkBuffer,
   &memRequ);

   VkMemoryAllocateFlagsInfo allocFlagsInfo;
   allocFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
   allocFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
   allocFlagsInfo.deviceMask = 0;
   allocFlagsInfo.pNext = NULL;

   VkMemoryAllocateInfo allocInfo;
   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
   allocInfo.allocationSize = memRequ.size;
   allocInfo.memoryTypeIndex = 
   loci_findMemoryType
   (memRequ.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
   allocInfo.pNext = &allocFlagsInfo;

   LOCI_CHECK_VULKAN
   (vkAllocateMemory
   (loci_vkDevice,
   &allocInfo,NULL,
   &buffer.vkDeviceMemory),
   "createBuffer",
   "vkAllocateMemory")

   LOCI_CHECK_VULKAN
   (vkBindBufferMemory
   (loci_vkDevice,
   buffer.vkBuffer,
   buffer.vkDeviceMemory,
   0),
   "createBuffer",
   "vkBindBufferMemory")

   VkBufferDeviceAddressInfoKHR bufferDeviceAI;
   bufferDeviceAI.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR;
   bufferDeviceAI.buffer = buffer.vkBuffer;
   bufferDeviceAI.pNext = NULL;
   buffer.deviceAddress = loci_vkGetBufferDeviceAddressKHR
   (loci_vkDevice, &bufferDeviceAI);

   return buffer;
}

void loci_destroyBuffer(Loci_Buffer buffer)
{
   vkFreeMemory
   (loci_vkDevice,
   buffer.vkDeviceMemory,
   NULL);

   vkDestroyBuffer
   (loci_vkDevice,
   buffer.vkBuffer,
   NULL);
}

Loci_BufferInterface loci_createBufferInterface
(uint32_t numberElements, uint32_t elementSize, 
VkBufferUsageFlags usage)
{
   Loci_BufferInterface buffer;
   buffer.numberElements = numberElements;
   buffer.elementSize = elementSize;

   VkBufferCreateInfo createInfo;
   createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
   createInfo.flags = 0;
   createInfo.size = numberElements * elementSize;
   createInfo.usage = usage | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT; 
   createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
   createInfo.queueFamilyIndexCount = 0;
   createInfo.pQueueFamilyIndices = NULL;
   createInfo.pNext = NULL;

   LOCI_CHECK_VULKAN
   (vkCreateBuffer
   (loci_vkDevice,
   &createInfo, 
   NULL,
   &buffer.vkBuffer),
   "createBuffer",
   "vkCreateBuffer")

   VkMemoryRequirements memRequ;
   vkGetBufferMemoryRequirements
   (loci_vkDevice,
   buffer.vkBuffer,  
   &memRequ);

   VkMemoryAllocateFlagsInfo allocFlagsInfo;
   allocFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
   allocFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
   allocFlagsInfo.deviceMask = 0;
   allocFlagsInfo.pNext = NULL;
   
   VkMemoryAllocateInfo allocInfo;
   allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
   allocInfo.allocationSize = memRequ.size;
   allocInfo.memoryTypeIndex = 
   loci_findMemoryType
   (memRequ.memoryTypeBits, 
   VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
   allocInfo.pNext = &allocFlagsInfo;

   LOCI_CHECK_VULKAN
   (vkAllocateMemory
   (loci_vkDevice,
   &allocInfo,NULL,
   &buffer.vkDeviceMemory),
   "createBuffer",
   "vkAllocateMemory")

   LOCI_CHECK_VULKAN
   (vkBindBufferMemory
   (loci_vkDevice,
   buffer.vkBuffer,
   buffer.vkDeviceMemory,
   0),
   "createBuffer",
   "vkBindBufferMemory")

   LOCI_CHECK_VULKAN
   (vkMapMemory
   (loci_vkDevice,
   buffer.vkDeviceMemory,
   0,numberElements*elementSize,
   0, &buffer.pMemory),
   "createBuffer",
   "vkMapMemory")

   VkBufferDeviceAddressInfoKHR bufferDeviceAI;
   bufferDeviceAI.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR;
   bufferDeviceAI.buffer = buffer.vkBuffer;
   bufferDeviceAI.pNext = NULL;
   buffer.deviceAddress = loci_vkGetBufferDeviceAddressKHR
   (loci_vkDevice, &bufferDeviceAI);

   return buffer;
}

void loci_destroyBufferInterface(Loci_BufferInterface buffer)
{
   vkUnmapMemory
   (loci_vkDevice,
   buffer.vkDeviceMemory);

   vkFreeMemory
   (loci_vkDevice,
   buffer.vkDeviceMemory,
   NULL);

   vkDestroyBuffer
   (loci_vkDevice,
   buffer.vkBuffer,
   NULL);
}