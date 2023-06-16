#version 460
#extension GL_EXT_ray_tracing : enable

#include "structs.glsl"

layout(location = 0) rayPayloadInEXT Payload payload;

layout( push_constant ) uniform constants
{
	uint32_t numberPointLights;
} pushConstants;

void main()
{
}