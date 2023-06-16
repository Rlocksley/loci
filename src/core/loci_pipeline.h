 #ifndef LOCI_PIPELINE_H
 #define LOCI_PIPELINE_H

#include "loci_descriptorSet.h"
#include "loci_shader.h"

typedef struct Loci_RayPipelinePushConstants
{
    uint32_t numberPointLights;
}Loci_RayPipelinePushConstants;

typedef struct Loci_RayPipeline
{
    VkPipelineLayout vkPipelineLayout;
    VkPipeline vkPipeline;
}Loci_RayPipeline;

Loci_RayPipeline loci_createRayPipeline
(Loci_RayDescriptorSet descriptorSet,
Loci_Shader generationShader,
Loci_Shader missShader[], uint32_t numberMissShader,
Loci_Shader closestHitShader[], uint32_t numberClosestHitShader,
uint32_t maxRecursionDepth);
void loci_destroyRayPipeline(Loci_RayPipeline pipeline);

#endif