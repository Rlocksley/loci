#extension GL_EXT_ray_tracing : enable
#extension GL_GOOGLE_include_directive : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int32 : enable
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_buffer_reference2 : enable
#extension GL_EXT_nonuniform_qualifier : enable


struct Payload
{
    vec3 hitValue;
    vec3 hitPosition;
    int lightNumber;
    vec3 lightColor;
};

struct Vertex
{
    vec3 position;
    vec3 normal;
    vec3 color;
    vec2 uv;
};

struct Object
{
    uint64_t vertexAddress;
    uint64_t indexAddress;
    int32_t textureIndex;
    vec3 ambient;
    float roughness;
    float metallic;
    float reflection;
};

struct PointLight
{
    vec4 color;
    vec4 translation;
};