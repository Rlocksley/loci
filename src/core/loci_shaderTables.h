#ifndef LOCI_SHADERTABLES_H
#define LOCI_SHADERTABLES_H

#include "loci_device.h"
#include "loci_buffer.h"
#include "loci_pipeline.h"
#include "loci_commandBuffer.h"
#include "loci_fence.h"

typedef struct Loci_ShaderTables
{
    Loci_Buffer genShaderTableBuffer;
    Loci_Buffer missShaderTableBuffer;
    Loci_Buffer chitShaderTableBuffer;
}Loci_ShaderTables;

Loci_ShaderTables loci_createShaderTables
(Loci_RayPipeline pipeline,
uint32_t numberMissShaders,
uint32_t numberClosestHitShaders);
void loci_destroyShaderTables(Loci_ShaderTables shaderTables);

#endif