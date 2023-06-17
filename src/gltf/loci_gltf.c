#include "loci_gltf.h"

#define CGLTF_IMPLEMENTATION
#include "./cgltf/cgltf.h"

const cgltf_node* loci_findNodeContainingMesh(const cgltf_data* data, const cgltf_mesh* mesh) {
    for (cgltf_size i = 0; i < data->nodes_count; ++i) {
        const cgltf_node* node = &data->nodes[i];
        if (node->mesh == mesh) {
            return node;
        }
    }
    return NULL;
}

void loci_getWorldTransform(const cgltf_node* pNode, mat4 transform)
{
    if(pNode->parent)
    {
        loci_getWorldTransform(pNode->parent, transform);
    }

    mat4 localTransform;
    localTransform[0][0] = pNode->matrix[0];
    localTransform[0][1] = pNode->matrix[1];
    localTransform[0][2] = pNode->matrix[2];
    localTransform[0][3] = pNode->matrix[3];
    localTransform[1][0] = pNode->matrix[4];
    localTransform[1][1] = pNode->matrix[5];
    localTransform[1][2] = pNode->matrix[6];
    localTransform[1][3] = pNode->matrix[7];
    localTransform[2][0] = pNode->matrix[8];
    localTransform[2][1] = pNode->matrix[9];
    localTransform[2][2] = pNode->matrix[10];
    localTransform[2][3] = pNode->matrix[11];
    localTransform[3][0] = pNode->matrix[12];
    localTransform[3][1] = pNode->matrix[13];
    localTransform[3][2] = pNode->matrix[14];
    localTransform[3][3] = pNode->matrix[15];

    glm_mat4_mul(transform, localTransform, transform);
}


void loci_createMeshModel
(ecs_entity_t entity, 
const char* pDirectoryFilePath, const char* pFilePath,
vec3 ambient, float roughness, float metallic, float reflection)
{
    
    cgltf_options options;
    memset(&options, 0, sizeof(cgltf_options));
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, pFilePath, &data);
    if(result != cgltf_result_success)
    {
        LOCI_LOGE("createMeshModel", "cgltf_parse_file() failed for", pFilePath)
    }

    result = cgltf_load_buffers(&options, data, pFilePath);
    if(result != cgltf_result_success)
    {
        LOCI_LOGE("createMeshModel", "cgltf_load_buffers() failed for", pFilePath)
    }

    result = cgltf_validate(data);
    if(result != cgltf_result_success)
    {
        LOCI_LOGE("createMeshModel", "cgltf_validate failed for data of", pFilePath)
    }

    ecs_entity_t textures[data->textures_count];
    char* pFilePaths[data->textures_count];
    for(cgltf_size i = 0; i < data->textures_count; i++)
    {
        const cgltf_texture* gltfTexture = &data->textures[i];
        char* pTextureFilePath = (char*) malloc((strlen(pDirectoryFilePath)+strlen(gltfTexture->image->uri)+1)*sizeof(char));
        if(pTextureFilePath == NULL)
        {
            LOCI_LOGE("createMeshModel", "malloc() for pTextureFilePath", "failed")
        }
        pFilePaths[i] = pTextureFilePath;

        memcpy(pTextureFilePath, pDirectoryFilePath, strlen(pDirectoryFilePath)*sizeof(char));
        memcpy(pTextureFilePath+strlen(pDirectoryFilePath), gltfTexture->image->uri, strlen(gltfTexture->image->uri)*sizeof(char));
        memcpy(pTextureFilePath+strlen(pDirectoryFilePath)+strlen(gltfTexture->image->uri), "\0", sizeof(char));

        
        textures[i] = loci_createEntity()
        loci_addPair(entity, loci_isModelOfTexture, textures[i])
        loci_createTexture(textures[i], pTextureFilePath);
    }

    for (cgltf_size i = 0; i < data->meshes_count; ++i) 
    {
        const cgltf_mesh* gltfMesh = &data->meshes[i];
        for (cgltf_size j = 0; j < gltfMesh->primitives_count; ++j) 
        {
            const cgltf_primitive* primitive = &gltfMesh->primitives[j];
            
            // Retrieve number of vertices and indices
            cgltf_size numVertices = primitive->attributes[0].data->count;
            cgltf_size numIndices = primitive->indices->count;
            
            

            bool bPosition = false;
            bool bNormal = false;
            bool bColor = false;
            bool bTextCoord = false;

            float* pPositionData = (float*) malloc(sizeof(float)*numVertices*3);
            float* pNormalData = (float*) malloc(sizeof(float)*numVertices*3);
            float* pColorData = (float*) malloc(sizeof(float)*numVertices*3);
            float* pTextureData = (float*) malloc(sizeof(float)*numVertices*2);

            for(cgltf_size k = 0; k < primitive->attributes_count; k++)
            {
            // Process position attribute
            if (primitive->attributes[k].type == cgltf_attribute_type_position && !bPosition) 
            {
                printf("Primitive\n");

                const cgltf_accessor* accessor = primitive->attributes[k].data;
                
                cgltf_accessor_unpack_floats(accessor, pPositionData, numVertices*3);


                bPosition = true;
            }
            
            // Process normal attribute
            if (primitive->attributes[k].type == cgltf_attribute_type_normal && !bNormal) 
            {
                const cgltf_accessor* accessor = primitive->attributes[k].data;

                cgltf_accessor_unpack_floats(accessor, pNormalData, numVertices*3);

                bNormal = true;
            }
            
            // Process color attribute
            if (primitive->attributes[k].type == cgltf_attribute_type_color && !bColor) 
            {
                const cgltf_accessor* accessor = primitive->attributes[k].data;

                cgltf_accessor_unpack_floats(accessor, pColorData, numVertices*3);
            
                bColor = true;
            }

            if(primitive->attributes[k].type == cgltf_attribute_type_texcoord && !bTextCoord)
            {
                const cgltf_accessor* accessor = primitive->attributes[k].data;

                cgltf_accessor_unpack_floats(accessor, pTextureData, numVertices*2);

                bTextCoord = true;
            }

            }

            if(bPosition)
            {

                Loci_Vertex* pVertices = (Loci_Vertex*) malloc(sizeof(Loci_Vertex)*numVertices);
                if(pVertices == NULL)
                {
                    LOCI_LOGE("createMeshModel", "malloc() for pVertices", "failed")
                }

                uint32_t* pIndices = (uint32_t*) malloc(sizeof(uint32_t)*numIndices);
                if(pIndices == NULL)
                {
                    LOCI_LOGE("createMeshModel", "malloc() for pIndices", "failed")
                }

                for(uint32_t n = 0; n < numVertices; n++)
                {
                    glm_vec3_copy((vec3){pPositionData[3*n],pPositionData[3*n+1],pPositionData[3*n+2]},
                    pVertices[n].position);
                }

                for(uint32_t n = 0; n < numVertices; n++)
                {
                    glm_vec3_copy((vec3){pNormalData[3*n],pNormalData[3*n+1],pNormalData[3*n+2]},
                    pVertices[n].normal);
                }

                for(uint32_t n = 0; n < numVertices; n++)
                {
                    glm_vec3_copy((vec3){pColorData[3*n],pColorData[3*n+1],pColorData[3*n+2]},
                    pVertices[n].color);
                }
            
                for(uint32_t n = 0; n < numVertices; n++)
                {
                    glm_vec2_copy((vec3){pTextureData[2*n],pTextureData[2*n+1]},
                    pVertices[n].uv);
                }
            

                // Process indices

                const cgltf_accessor* accessor = primitive->indices;
                cgltf_uint* pGLTFindices = (cgltf_uint*) malloc(sizeof(cgltf_uint)*numIndices);


                cgltf_accessor_unpack_indices(accessor, pGLTFindices, numIndices);

                for(cgltf_size n = 0; n < numIndices; n++)
                {
                    pIndices[n] = (uint32_t) pGLTFindices[n];
                }

                //Mesh Entity
                ecs_entity_t meshEntity = 0;
                cgltf_float volumeColor[3];

                //Textures
                cgltf_texture* pTexture;
                if(primitive->material && primitive->material->has_pbr_metallic_roughness &&
                (pTexture = primitive->material->pbr_metallic_roughness.base_color_texture.texture))
                {
                    char pTextureFilePath[strlen(pDirectoryFilePath)+strlen(pTexture->image->uri)+1];
                    strcpy(pTextureFilePath, pDirectoryFilePath);
                    strcpy(pTextureFilePath+strlen(pDirectoryFilePath), pTexture->image->uri);

                    for(cgltf_size n = 0; n < data->textures_count; n++)
                    {
                        if(strcmp(pTextureFilePath, pFilePaths[n]) == 0)
                        {
                            meshEntity = ecs_new_w_pair(loci_pWorld, EcsIsA, textures[n]);                        
                            break;
                        }
                    }
                }
                else if(primitive->material && primitive->material->has_pbr_specular_glossiness &&
                (pTexture = primitive->material->pbr_specular_glossiness.diffuse_texture.texture))
                {
                    char pTextureFilePath[strlen(pDirectoryFilePath)+strlen(pTexture->image->uri)+1];
                    strcpy(pTextureFilePath, pDirectoryFilePath);
                    strcpy(pTextureFilePath+strlen(pDirectoryFilePath), pTexture->image->uri);

                    for(cgltf_size n = 0; n < data->textures_count; n++)
                    {
                        if(strcmp(pTextureFilePath, pFilePaths[n]) == 0)
                        {
                            meshEntity = ecs_new_w_pair(loci_pWorld, EcsIsA, textures[n]);                        
                            break;
                        }
                    }
                }
                else if(primitive->material && primitive->material->has_clearcoat &&
                (pTexture = primitive->material->clearcoat.clearcoat_texture.texture))
                {
                    char pTextureFilePath[strlen(pDirectoryFilePath)+strlen(pTexture->image->uri)+1];
                    strcpy(pTextureFilePath, pDirectoryFilePath);
                    strcpy(pTextureFilePath+strlen(pDirectoryFilePath), pTexture->image->uri);
    
                    for(cgltf_size n = 0; n < data->textures_count; n++)
                    {
                        if(strcmp(pTextureFilePath, pFilePaths[n]) == 0)
                        {
                            meshEntity = ecs_new_w_pair(loci_pWorld, EcsIsA, textures[n]);                        
                            break;
                        }
                    }
                }
                else if(primitive->material && primitive->material->has_transmission &&
                (pTexture = primitive->material->transmission.transmission_texture.texture))
                {
                    char pTextureFilePath[strlen(pDirectoryFilePath)+strlen(pTexture->image->uri)+1];
                    strcpy(pTextureFilePath, pDirectoryFilePath);
                    strcpy(pTextureFilePath+strlen(pDirectoryFilePath), pTexture->image->uri);

                    for(cgltf_size n = 0; n < data->textures_count; n++)
                    {
                        if(strcmp(pTextureFilePath, pFilePaths[n]) == 0)
                        {
                            meshEntity = ecs_new_w_pair(loci_pWorld, EcsIsA, textures[n]);                        
                            break;
                        }
                    }
                }
                else if(primitive->material && primitive->material->has_specular &&
                (pTexture = primitive->material->specular.specular_color_texture.texture))
                {
                    char pTextureFilePath[strlen(pDirectoryFilePath)+strlen(pTexture->image->uri)+1];
                    strcpy(pTextureFilePath, pDirectoryFilePath);
                    strcpy(pTextureFilePath+strlen(pDirectoryFilePath), pTexture->image->uri);

                    for(cgltf_size n = 0; n < data->textures_count; n++)
                    {
                        if(strcmp(pTextureFilePath, pFilePaths[n]) == 0)
                        {
                            meshEntity = ecs_new_w_pair(loci_pWorld, EcsIsA, textures[n]);                        
                            break;
                        }
                    }
                }
                else if(primitive->material && primitive->material->has_iridescence &&
                (pTexture = primitive->material->iridescence.iridescence_texture.texture))
                {
                    char pTextureFilePath[strlen(pDirectoryFilePath)+strlen(pTexture->image->uri)+1];
                    strcpy(pTextureFilePath, pDirectoryFilePath);
                    strcpy(pTextureFilePath+strlen(pDirectoryFilePath), pTexture->image->uri);

                    for(cgltf_size n = 0; n < data->textures_count; n++)
                    {
                        if(strcmp(pTextureFilePath, pFilePaths[n]) == 0)
                        {
                            meshEntity = ecs_new_w_pair(loci_pWorld, EcsIsA, textures[n]);                        
                            break;
                        }
                    }
                }
                else if(primitive->material && primitive->material->has_sheen &&
                (pTexture = primitive->material->sheen.sheen_color_texture.texture))
                {
                    char pTextureFilePath[strlen(pDirectoryFilePath)+strlen(pTexture->image->uri)+1];
                    strcpy(pTextureFilePath, pDirectoryFilePath);
                    strcpy(pTextureFilePath+strlen(pDirectoryFilePath), pTexture->image->uri);

                    for(cgltf_size n = 0; n < data->textures_count; n++)
                    {
                        if(strcmp(pTextureFilePath, pFilePaths[n]) == 0)
                        {
                            meshEntity = ecs_new_w_pair(loci_pWorld, EcsIsA, textures[n]);                        
                            break;
                        }
                    }
                }
                else if(primitive->material && primitive->material->has_anisotropy &&
                (pTexture = primitive->material->anisotropy.anisotropy_texture.texture))
                {
                    char pTextureFilePath[strlen(pDirectoryFilePath)+strlen(pTexture->image->uri)+1];
                    strcpy(pTextureFilePath, pDirectoryFilePath);
                    strcpy(pTextureFilePath+strlen(pDirectoryFilePath), pTexture->image->uri);

                    for(cgltf_size n = 0; n < data->textures_count; n++)
                    {
                        if(strcmp(pTextureFilePath, pFilePaths[n]) == 0)
                        {
                            meshEntity = ecs_new_w_pair(loci_pWorld, EcsIsA, textures[n]);                        
                            break;
                        }
                    }
                }
                else if(primitive->material && 
                (pTexture = primitive->material->occlusion_texture.texture))
                {
                    char pTextureFilePath[strlen(pDirectoryFilePath)+strlen(pTexture->image->uri)+1];
                    strcpy(pTextureFilePath, pDirectoryFilePath);
                    strcpy(pTextureFilePath+strlen(pDirectoryFilePath), pTexture->image->uri);

                    for(cgltf_size n = 0; n < data->textures_count; n++)
                    {
                        if(strcmp(pTextureFilePath, pFilePaths[n]) == 0)
                        {
                            meshEntity = ecs_new_w_pair(loci_pWorld, EcsIsA, textures[n]);                        
                            break;
                        }
                    }
                }
                else if(primitive->material && 
                (pTexture = primitive->material->emissive_texture.texture))
                {
                    char pTextureFilePath[strlen(pDirectoryFilePath)+strlen(pTexture->image->uri)+1];
                    strcpy(pTextureFilePath, pDirectoryFilePath);
                    strcpy(pTextureFilePath+strlen(pDirectoryFilePath), pTexture->image->uri);

                    for(cgltf_size n = 0; n < data->textures_count; n++)
                    {
                        if(strcmp(pTextureFilePath, pFilePaths[n]) == 0)
                        {
                            meshEntity = ecs_new_w_pair(loci_pWorld, EcsIsA, textures[n]);                        
                            break;
                        }
                    }
                }
                else if(primitive->material && primitive->material->has_volume)
                {
                    volumeColor[0] =  primitive->material->volume.attenuation_color[0];
                    volumeColor[1] =  primitive->material->volume.attenuation_color[1];
                    volumeColor[2] =  primitive->material->volume.attenuation_color[2];

                    for(cgltf_size n = 0; n < numVertices; n++)
                    {
                        pVertices[n].color[0] = volumeColor[0];    
                        pVertices[n].color[1] = volumeColor[1];    
                        pVertices[n].color[2] = volumeColor[2];    
                    }
                }
                
                mat4 transform;
                glm_mat4_identity(transform);
                glm_rotate(transform, GLM_PI, (vec3){0.f,0.f,1.f});
                const cgltf_node* pNode = loci_findNodeContainingMesh(data, gltfMesh);
                if(pNode != NULL)
                {
                    loci_getWorldTransform(pNode, transform);
                }

                if(meshEntity == 0)
                {
                    meshEntity = loci_createEntity()
                }

                loci_addPair(entity, loci_isModelOfMesh, meshEntity);
                loci_createTransform2(meshEntity, transform);
                
                mat4 identity;
                glm_mat4_identity(identity);
                loci_createMesh(meshEntity, pVertices, numVertices, pIndices, numIndices, identity);
                loci_createMaterial(meshEntity, ambient , roughness, metallic, reflection);

                free(pIndices);
                free(pVertices);
            

            }

            free(pColorData);
            free(pNormalData);
            free(pPositionData);
        }
    }

    for(cgltf_size i = 0; i < data->textures_count; i++)
    {
        free((void*)pFilePaths[i]);
    }

    cgltf_free(data);
}

void loci_spawnMeshModelInScene(ecs_entity_t entity, ecs_entity_t scene)
{
    ecs_entity_t meshEntity;
    int index = 0;
    while((meshEntity = ecs_get_target(loci_pWorld, entity, loci_isModelOfMesh, index++)))
    {
        loci_spawnInScene(meshEntity, scene);
    }
}

void loci_despawnMeshModelInScene(ecs_entity_t entity, ecs_entity_t scene)
{
    ecs_entity_t meshEntity;
    int index = 0;
    while((meshEntity = ecs_get_target(loci_pWorld, entity, loci_isModelOfMesh, index++)))
    {
        loci_despawnInScene(meshEntity, scene);
    }
}


void loci_destroyMeshModel(ecs_entity_t entity)
{
    ecs_entity_t meshEntity;
    int32_t index = 0;
    while((meshEntity = ecs_get_target(loci_pWorld, entity, loci_isModelOfMesh, 0)))
    {

        loci_destroyMaterial(meshEntity);

        loci_destroyMesh(meshEntity);

        loci_destroyTransform(meshEntity);

        loci_removePair(entity, loci_isModelOfMesh, meshEntity);

        ecs_delete(loci_pWorld, meshEntity);
    }

    ecs_entity_t textureEntity;
    while((textureEntity = ecs_get_target(loci_pWorld, entity, loci_isModelOfTexture, 0)))
    {
        ECS_COMPONENT(loci_pWorld, Loci_Texture);

        loci_destroyTexture(textureEntity);

        loci_removePair(entity, loci_isModelOfTexture, textureEntity);

        ecs_delete(loci_pWorld, textureEntity);
    }
}

