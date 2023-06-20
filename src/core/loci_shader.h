#ifndef LOCI_SHADER_H
#define LOCI_SHADER_H

#include "loci_device.h"

typedef struct Loci_Shader
{
    uint32_t size;
    char* pCode;
}Loci_Shader;

extern char* loci_pShaderDirectory;

extern Loci_Shader loci_skeletonShader;

Loci_Shader loci_createShader(const char* filePath);
void loci_destroyShader(Loci_Shader shader);

VkShaderModule loci_createShaderModule(Loci_Shader shader);
void loci_destroyShaderModule(VkShaderModule shaderModule);

#endif