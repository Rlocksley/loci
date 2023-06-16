#include "loci_shader.h"


Loci_Shader loci_createShader(const char* filePath)
{
    FILE* pFile = fopen(filePath, "r");
    if(pFile == NULL)
    {
        LOCI_LOGE("createShader", "fopen() failed for", filePath)
    }

    fseek(pFile, 0L, SEEK_END);
    long size = ftell(pFile);
    fseek(pFile, 0L, SEEK_SET);

    Loci_Shader shader;
    shader.size = (uint32_t) size;
    
    shader.pCode = (char*) malloc(size * sizeof(char));
    if(shader.pCode == NULL)
    {
        LOCI_LOGE("createShader", "malloc() for pCode", "failed")
    }
    
    if(fread(shader.pCode, sizeof(char), size, pFile) != size)
    {
        LOCI_LOGE("createShader", "fread() for pCode", "failed")
    }

    fclose(pFile);

    return shader;
}

void loci_destroyShader(Loci_Shader shader)
{
    free(shader.pCode);
}

VkShaderModule loci_createShaderModule(Loci_Shader shader)
{
    VkShaderModule module;

    VkShaderModuleCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.codeSize = shader.size;
    createInfo.pCode = (uint32_t*) shader.pCode;
    createInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreateShaderModule
    (loci_vkDevice,
    &createInfo, NULL,
    &module),
    "createShaderModule",
    "vkCreateShaderModule")

    return module;
}

void loci_destroyShaderModule(VkShaderModule shaderModule)
{
    vkDestroyShaderModule
    (loci_vkDevice,
    shaderModule,
    NULL);
}


