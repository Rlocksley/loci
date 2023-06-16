#version 460
#extension GL_EXT_ray_tracing : enable
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int32 : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_buffer_reference2 : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include "structs.glsl"

//=========================================================================================================
//
// PBR Functions
//=========================================================================================================
const float PI = 3.14159265359;

vec3 PBR(vec3 hitPosition, vec3 hitNormal, vec3 viewDirection, vec3 lightDirection, vec3 albedo, float roughness, float metallic, vec3 Ao, vec3 reflection)
{
    vec3 N = normalize(hitNormal);
    vec3 V = normalize(viewDirection);
    vec3 L = normalize(lightDirection);
    vec3 H = normalize(L + V);

    // Diffuse
    float diffuse = max(dot(N, L), 0.0);

    // Specular
    float NdotH = max(dot(N, H), 0.0);
    float roughness2 = roughness * roughness;
    float specular = pow(NdotH, 1.0 / (roughness2 + 0.0001));
    specular = mix(0.0, specular, metallic);

    // Reflection
    vec3 R = reflect(V, N);
    float reflectivity = pow(max(dot(R, L), 0.0),roughness);
    reflectivity = mix(0.0, reflectivity, metallic);

    // Final color
    vec3 color = (albedo * (diffuse + specular + Ao)) + (reflection * reflectivity);
    return color;
}


//=======================================================================================

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
    
    vec3 hitColor;
    vec2 uvPosition;
    
    if(object.textureIndex > -1)
    {
        uvPosition = vertex0.uv*barycentricCoords[0]+vertex1.uv*barycentricCoords[1]+vertex2.uv*barycentricCoords[2];
        hitColor = texture(textureSamplers[nonuniformEXT(int(object.textureIndex))], uvPosition).xyz;
    }
    else
    {
        hitColor = vertex0.color*barycentricCoords[0]+vertex1.color*barycentricCoords[1]+vertex2.color*barycentricCoords[2];
    }

    vec3 hitPosition = vertex0.position*barycentricCoords[0]+vertex1.position*barycentricCoords[1]+vertex2.position*barycentricCoords[2];
    hitPosition = vec3(gl_ObjectToWorldEXT * vec4(hitPosition, 1.0));
    payload.hitPosition = hitPosition;
    
    vec3 hitNormal = vertex0.normal * barycentricCoords[0] + vertex1.normal * barycentricCoords[1] + vertex2.normal * barycentricCoords[2];
    hitNormal = normalize(vec3(hitNormal * gl_WorldToObjectEXT));

    //Reflection
    //-------------------
    vec3 direction = reflect(gl_WorldRayDirectionEXT, hitNormal);
    traceRayEXT(topLevelAS, gl_RayFlagsOpaqueEXT, 0xff, 2, 0, 2, hitPosition, camera.clip[0], direction, camera.clip[1], 0);
    //-------------------

    payload.hitValue *= object.reflection;

    //Shadowing
    //-------------------
    for(int i = 0; i < pushConstants.numberPointLights; i++)
    {
        payload.lightNumber = i;
        payload.lightColor = vec3(0.f,0.f,0.f);
        traceRayEXT(topLevelAS, gl_RayFlagsOpaqueEXT, 0xff, 1, 0, 1, hitPosition, camera.clip[0], normalize(pointLights.lights[i].translation.xyz-hitPosition), camera.clip[1], 0);
        payload.hitValue += 
        PBR(hitPosition, hitNormal, normalize(hitPosition-camera.position.xyz), normalize(pointLights.lights[i].translation.xyz-hitPosition), hitColor*payload.lightColor, object.roughness, object.metallic, object.ambient, payload.hitValue);
    }
    if(length(payload.hitValue) < 0.1)
    {
        payload.hitValue += hitColor * vec3(0.1,0.1,0.1);
    }
    //-------------------
}   