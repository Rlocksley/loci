#version 460
#extension GL_EXT_ray_tracing : enable
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int32 : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_buffer_reference2 : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include "structs.glsl"


layout(binding = 0, set = 0) uniform accelerationStructureEXT topLevelAS;

layout(binding = 2, set = 0) uniform CameraProperties 
{
	vec4 position;
    vec4 direction;
    mat4 viewInverse;
    mat4 projInverse;
    vec4 clip;
} camera;


layout(buffer_reference, scalar) buffer Vertices {Vertex v[]; };
layout(buffer_reference, scalar) buffer Indices {ivec3 i[]; };
layout(binding = 3, set = 0, scalar) buffer ObjectsBuffer
{
    Object obj[];
}objects;

layout(binding = 4, set = 0) uniform PointLightBuffer
{
    PointLight lights[16];
}pointLights;

layout(binding = 5, set = 0) uniform sampler2D textureSamplers[];

layout(location = 0) rayPayloadInEXT Payload payload;

hitAttributeEXT vec2 attribs;


layout( push_constant ) uniform constants
{
	uint32_t numberPointLights;
} pushConstants;


void main()
{
    Object object = objects.obj[gl_InstanceCustomIndexEXT];
    Vertices vertices = Vertices(object.vertexAddress);
    Indices indices = Indices(object.indexAddress);
    ivec3 index = indices.i[gl_PrimitiveID];

    Vertex vertex0 = vertices.v[index[0]];
    Vertex vertex1 = vertices.v[index[1]];
    Vertex vertex2 = vertices.v[index[2]];

    const vec3 barycentricCoords = vec3(1.0f - attribs.x - attribs.y, attribs.x, attribs.y);

    vec3 hitPosition = vertex0.position.xyz*barycentricCoords[0]+vertex1.position.xyz*barycentricCoords[1]+vertex2.position.xyz*barycentricCoords[2];
    hitPosition = vec3(gl_ObjectToWorldEXT * vec4(hitPosition, 1.0));
        
    if(length(payload.hitPosition - hitPosition) > length(payload.hitPosition - pointLights.lights[payload.lightNumber].translation.xyz))
    {
        payload.lightColor += pointLights.lights[payload.lightNumber].color.xyz ;
    }

}   