#include "loci_image.h"

void loci_createRayImage()
{
    VkImageCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    createInfo.format = loci_vkSurfaceFormatKHR.format;
    createInfo.extent.width = loci_vkExtent2D.width;
    createInfo.extent.height = loci_vkExtent2D.height;
    createInfo.extent.depth = 1;
    createInfo.mipLevels = 1;
    createInfo.arrayLayers = 1;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    createInfo.usage = VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = NULL;
    createInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreateImage
    (loci_vkDevice,
    &createInfo, NULL,
    &loci_rayImage.vkImage),
    "createRayImage",
    "vkCreateImage")

    VkMemoryRequirements memRequ;
    vkGetImageMemoryRequirements
    (loci_vkDevice,
    loci_rayImage.vkImage,
    &memRequ);

    VkMemoryAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequ.size;
    allocInfo.memoryTypeIndex = 
    loci_findMemoryType
    (memRequ.memoryTypeBits,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    allocInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkAllocateMemory
    (loci_vkDevice,
    &allocInfo, NULL,
    &loci_rayImage.vkDeviceMemory),
    "createRayImage",
    "vkAllocateMemory")

    LOCI_CHECK_VULKAN
    (vkBindImageMemory
    (loci_vkDevice,
    loci_rayImage.vkImage,
    loci_rayImage.vkDeviceMemory,
    0),
    "createRayImage",
    "vkBindImageMemory")

    VkImageViewCreateInfo ivCreateInfo;
    ivCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    ivCreateInfo.flags = 0;
    ivCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    ivCreateInfo.format = loci_vkSurfaceFormatKHR.format;
    ivCreateInfo.image = loci_rayImage.vkImage;
    ivCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    ivCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    ivCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    ivCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    ivCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    ivCreateInfo.subresourceRange.baseMipLevel = 0;
    ivCreateInfo.subresourceRange.levelCount = 1;
    ivCreateInfo.subresourceRange.baseArrayLayer = 0;
    ivCreateInfo.subresourceRange.layerCount = 1;
    ivCreateInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreateImageView
    (loci_vkDevice,
    &ivCreateInfo, NULL,
    &loci_rayImage.vkImageView),
    "createRayImage",
    "vkCreateImageView")

    VkImageMemoryBarrier barrier;
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = 0;
    barrier.image = loci_rayImage.vkImage;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.pNext = NULL;

    loci_beginCommand(loci_buildVkCommandBuffer);

    vkCmdPipelineBarrier
    (loci_buildVkCommandBuffer,
    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR,
    0,
    0, NULL,
    0, NULL,
    1, &barrier);

    loci_endCommand(loci_buildVkCommandBuffer);

    loci_submitSingleCommand
    (loci_buildVkCommandBuffer, loci_buildVkFence);

    #ifdef LOCI_DEBUG
    LOCI_LOGI("RayImage", "created", "")
    #endif
}

void loci_destroyRayImage()
{
    vkDestroyImageView
    (loci_vkDevice,
    loci_rayImage.vkImageView,
    NULL);

    vkFreeMemory
    (loci_vkDevice,
    loci_rayImage.vkDeviceMemory,
    NULL);

    vkDestroyImage
    (loci_vkDevice,
    loci_rayImage.vkImage,
    NULL);
}


Loci_Image loci_createImage(const char* filePath)
{
    int _width, _height, channels;
    stbi_uc* pixels = stbi_load
    (filePath, &_width, &_height, &channels, STBI_rgb_alpha);
    if(pixels == NULL)
    {
        LOCI_LOGE("createImage", "stbi_load() failed for", filePath)
    }

    uint32_t width = (uint32_t) _width;
    uint32_t height = (uint32_t) _height;

    Loci_BufferInterface copyBuffer = 
    loci_createBufferInterface
    (width*height, 4, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    memcpy(copyBuffer.pMemory, pixels, width*height*4);

    stbi_image_free(pixels);

    Loci_Image image;

    VkImageCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    createInfo.extent.width = width;
    createInfo.extent.height = height;
    createInfo.extent.depth = 1;
    createInfo.mipLevels = 1;
    createInfo.arrayLayers = 1;
    createInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    createInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices = NULL;
    createInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreateImage
    (loci_vkDevice,
    &createInfo, NULL,
    &image.vkImage),
    "createImage",
    "vkCreateImage")

    VkMemoryRequirements memRequ;
    vkGetImageMemoryRequirements
    (loci_vkDevice,
    image.vkImage,
    &memRequ);

    VkMemoryAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequ.size;
    allocInfo.memoryTypeIndex = 
    loci_findMemoryType
    (memRequ.memoryTypeBits,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    allocInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkAllocateMemory
    (loci_vkDevice,
    &allocInfo, NULL,
    &image.vkDeviceMemory),
    "createImage",
    "vkAllocateMemory")

    LOCI_CHECK_VULKAN
    (vkBindImageMemory
    (loci_vkDevice,
    image.vkImage,
    image.vkDeviceMemory,
    0),
    "createImage",
    "vkBindImageMemory")

    VkImageViewCreateInfo ivCreateInfo;
    ivCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    ivCreateInfo.flags = 0;
    ivCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    ivCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    ivCreateInfo.image = image.vkImage;
    ivCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    ivCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    ivCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    ivCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    ivCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    ivCreateInfo.subresourceRange.baseMipLevel = 0;
    ivCreateInfo.subresourceRange.levelCount = 1;
    ivCreateInfo.subresourceRange.baseArrayLayer = 0;
    ivCreateInfo.subresourceRange.layerCount = 1;
    ivCreateInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreateImageView
    (loci_vkDevice,
    &ivCreateInfo, NULL,
    &image.vkImageView),
    "createImage",
    "vkCreateImageView")
    
    VkSamplerCreateInfo samplerCreateInfo;
    samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerCreateInfo.flags = 0;
    samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
    samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerCreateInfo.anisotropyEnable = VK_TRUE;
    samplerCreateInfo.maxAnisotropy = loci_vkPhysicalDeviceProperties.limits.maxSamplerAnisotropy;
    samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
    samplerCreateInfo.compareEnable = VK_FALSE;
    samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCreateInfo.mipLodBias = 0.f;
    samplerCreateInfo.minLod = 0.f;
    samplerCreateInfo.maxLod = 0.f;
    samplerCreateInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreateSampler
    (loci_vkDevice,
    &samplerCreateInfo, 
    NULL,
    &image.vkSampler),
    "createImage",
    "vkCreateSampler")

    VkImageMemoryBarrier barrier0;
    barrier0.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier0.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier0.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier0.srcAccessMask = 0;
    barrier0.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier0.image = image.vkImage;
    barrier0.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier0.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier0.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier0.subresourceRange.baseMipLevel = 0;
    barrier0.subresourceRange.levelCount = 1;
    barrier0.subresourceRange.baseArrayLayer = 0;
    barrier0.subresourceRange.layerCount = 1;
    barrier0.pNext = NULL;

    VkBufferImageCopy copy;
    copy.bufferOffset = 0;
    copy.bufferRowLength = 0;
    copy.bufferImageHeight = 0;
    copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy.imageSubresource.mipLevel = 0;
    copy.imageSubresource.baseArrayLayer = 0;
    copy.imageSubresource.layerCount = 1;
    copy.imageOffset.x = 0;
    copy.imageOffset.y = 0;
    copy.imageOffset.z = 0;
    copy.imageExtent.width = width;
    copy.imageExtent.height = height;
    copy.imageExtent.depth = 1;

    VkImageMemoryBarrier barrier1;
    barrier1.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier1.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier1.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier1.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier1.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    barrier1.image = image.vkImage;
    barrier1.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier1.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier1.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier1.subresourceRange.baseMipLevel = 0;
    barrier1.subresourceRange.levelCount = 1;
    barrier1.subresourceRange.baseArrayLayer = 0;
    barrier1.subresourceRange.layerCount = 1;
    barrier1.pNext = NULL;

    loci_beginCommand(loci_buildVkCommandBuffer);

    vkCmdPipelineBarrier
    (loci_buildVkCommandBuffer,
    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    VK_PIPELINE_STAGE_TRANSFER_BIT,
    0,
    0, NULL,
    0, NULL,
    1, &barrier0);

    vkCmdCopyBufferToImage
    (loci_buildVkCommandBuffer,
    copyBuffer.vkBuffer,
    image.vkImage,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    1, &copy);

    vkCmdPipelineBarrier
    (loci_buildVkCommandBuffer,
    VK_PIPELINE_STAGE_TRANSFER_BIT,
    VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR,
    0,
    0, NULL,
    0, NULL,
    1, &barrier1);

    loci_endCommand(loci_buildVkCommandBuffer);

    loci_submitSingleCommand
    (loci_buildVkCommandBuffer, loci_buildVkFence);

    loci_destroyBufferInterface(copyBuffer);

    return image;
}

void loci_destroyImage(Loci_Image image)
{
    vkDestroySampler
    (loci_vkDevice,
    image.vkSampler,
    NULL);

    vkDestroyImageView
    (loci_vkDevice,
    image.vkImageView,
    NULL);

    vkFreeMemory
    (loci_vkDevice,
    image.vkDeviceMemory,
    NULL);

    vkDestroyImage
    (loci_vkDevice,
    image.vkImage,
    NULL);
}