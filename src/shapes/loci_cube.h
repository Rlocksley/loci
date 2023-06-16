#ifndef LOCI_MESH_H
#define LOCI_MESH_H
#include "../ecs/loci_mesh.h"
#endif


typedef struct Loci_Cube
{
    uint32_t numberVertices;
    Loci_Vertex* pVertices;
    uint32_t numberIndices;
    uint32_t* pIndices;
}Loci_Cube;


Loci_Cube loci_createCube(float width, float height, float depth, vec3 color)
{
    Loci_Cube cube;
    cube.numberVertices = 24;
    cube.pVertices = (Loci_Vertex*) malloc(sizeof(Loci_Vertex)*24);
    cube.numberIndices = 36;
    cube.pIndices = (uint32_t*) malloc(sizeof(uint32_t)*36);

    //Front face
    glm_vec3_copy((vec3){-0.5f,-0.5f,0.5f}, cube.pVertices[0].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[0].position,cube.pVertices[0].position);
    glm_vec3_copy((vec3){0.f,0.f,1.f}, cube.pVertices[0].normal);
    glm_vec3_copy(color, cube.pVertices[0].color);
    glm_vec2_copy((vec2){0.f,0.f}, cube.pVertices[0].uv);

    
    glm_vec3_copy((vec3){0.5f,-0.5f, 0.5f}, cube.pVertices[1].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[1].position,cube.pVertices[1].position);
    glm_vec3_copy((vec3){0.f,0.f,1.f}, cube.pVertices[1].normal);
    glm_vec3_copy(color, cube.pVertices[1].color);
    glm_vec2_copy((vec2){1.f,0.f}, cube.pVertices[1].uv);

    
    glm_vec3_copy((vec3){-0.5f,0.5f,0.5f}, cube.pVertices[2].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[2].position,cube.pVertices[2].position);
    glm_vec3_copy((vec3){0.f,0.f,1.f}, cube.pVertices[2].normal);
    glm_vec3_copy(color, cube.pVertices[2].color);
    glm_vec2_copy((vec2){0.f,1.f}, cube.pVertices[2].uv);

    
    glm_vec3_copy((vec3){0.5f,0.5f,0.5f}, cube.pVertices[3].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[3].position,cube.pVertices[3].position);
    glm_vec3_copy((vec3){0.f,0.f,1.f}, cube.pVertices[3].normal);
    glm_vec3_copy(color, cube.pVertices[3].color);
    glm_vec2_copy((vec2){0.f,0.f}, cube.pVertices[3].uv);

    //back face
    glm_vec3_copy((vec3){0.5f,-0.5f,-0.5f}, cube.pVertices[4].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[4].position,cube.pVertices[4].position);
    glm_vec3_copy((vec3){0.f,0.f,-1.f}, cube.pVertices[4].normal);
    glm_vec3_copy(color, cube.pVertices[4].color);
    glm_vec2_copy((vec2){0.f,0.f}, cube.pVertices[4].uv);

    
    glm_vec3_copy((vec3){-0.5f,-0.5f,-0.5f}, cube.pVertices[5].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[5].position,cube.pVertices[5].position);
    glm_vec3_copy((vec3){0.f,0.f,-1.f}, cube.pVertices[5].normal);
    glm_vec3_copy(color, cube.pVertices[5].color);
    glm_vec2_copy((vec2){1.f,0.f}, cube.pVertices[5].uv);

    
    glm_vec3_copy((vec3){0.5f,0.5f,-0.5f}, cube.pVertices[6].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[6].position,cube.pVertices[6].position);
    glm_vec3_copy((vec3){0.f,0.f,-1.f}, cube.pVertices[6].normal);
    glm_vec3_copy(color, cube.pVertices[6].color);
    glm_vec2_copy((vec2){0.f,1.f}, cube.pVertices[6].uv);

    
    glm_vec3_copy((vec3){-0.5f,0.5f,-0.5f}, cube.pVertices[7].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[7].position,cube.pVertices[7].position);
    glm_vec3_copy((vec3){0.f,0.f,-1.f}, cube.pVertices[7].normal);
    glm_vec3_copy(color, cube.pVertices[7].color);
    glm_vec2_copy((vec2){1.f,1.f}, cube.pVertices[7].uv);

    //left face
    glm_vec3_copy((vec3){-0.5f,-0.5f,-0.5f}, cube.pVertices[8].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[8].position,cube.pVertices[8].position);
    glm_vec3_copy((vec3){-1.f,0.f,0.f}, cube.pVertices[8].normal);
    glm_vec3_copy(color, cube.pVertices[8].color);
    glm_vec2_copy((vec2){0.f,0.f}, cube.pVertices[8].uv);

    
    glm_vec3_copy((vec3){-0.5f,-0.5f,0.5f}, cube.pVertices[9].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[9].position,cube.pVertices[9].position);
    glm_vec3_copy((vec3){-1.f,0.f,0.f}, cube.pVertices[9].normal);
    glm_vec3_copy(color, cube.pVertices[9].color);
    glm_vec2_copy((vec2){1.f,0.f}, cube.pVertices[9].uv);

    
    glm_vec3_copy((vec3){-0.5f,0.5f,-0.5f}, cube.pVertices[10].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[10].position,cube.pVertices[10].position);
    glm_vec3_copy((vec3){-1.f,0.f,0.f}, cube.pVertices[10].normal);
    glm_vec3_copy(color, cube.pVertices[10].color);
    glm_vec2_copy((vec2){0.f,1.f}, cube.pVertices[10].uv);

    
    glm_vec3_copy((vec3){-0.5f,0.5f,0.5f}, cube.pVertices[11].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[11].position,cube.pVertices[11].position);
    glm_vec3_copy((vec3){-1.f,0.f,0.f}, cube.pVertices[11].normal);
    glm_vec3_copy(color, cube.pVertices[11].color);
    glm_vec2_copy((vec2){1.f,1.f}, cube.pVertices[11].uv);

    //Right face
    glm_vec3_copy((vec3){0.5f,-0.5f,0.5f}, cube.pVertices[12].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[12].position,cube.pVertices[12].position);
    glm_vec3_copy((vec3){1.f,0.f,0.f}, cube.pVertices[12].normal);
    glm_vec3_copy(color, cube.pVertices[12].color);
    glm_vec2_copy((vec2){0.f,0.f}, cube.pVertices[12].uv);

    
    glm_vec3_copy((vec3){0.5f,-0.5f,-0.5f}, cube.pVertices[13].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[13].position,cube.pVertices[13].position);
    glm_vec3_copy((vec3){1.f,0.f,0.f}, cube.pVertices[13].normal);
    glm_vec3_copy(color, cube.pVertices[13].color);
    glm_vec2_copy((vec2){1.f,0.f}, cube.pVertices[13].uv);

    
    glm_vec3_copy((vec3){0.5f,0.5f,0.5f}, cube.pVertices[14].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[14].position,cube.pVertices[14].position);
    glm_vec3_copy((vec3){1.f,0.f,0.f}, cube.pVertices[14].normal);
    glm_vec3_copy(color, cube.pVertices[14].color);
    glm_vec2_copy((vec2){0.f,1.f}, cube.pVertices[14].uv);

    
    glm_vec3_copy((vec3){0.5f,0.5f,-0.5f}, cube.pVertices[15].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[15].position,cube.pVertices[15].position);
    glm_vec3_copy((vec3){1.f,0.f,0.f}, cube.pVertices[15].normal);
    glm_vec3_copy(color, cube.pVertices[15].color);
    glm_vec2_copy((vec2){1.f,1.f}, cube.pVertices[15].uv);

    //Top Face
    glm_vec3_copy((vec3){-0.5f,0.5f,0.5f}, cube.pVertices[16].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[16].position,cube.pVertices[16].position);
    glm_vec3_copy((vec3){0.f,1.f,0.f}, cube.pVertices[16].normal);
    glm_vec3_copy(color, cube.pVertices[16].color);
    glm_vec2_copy((vec2){0.f,0.f}, cube.pVertices[16].uv);

    
    glm_vec3_copy((vec3){0.5f,0.5f,0.5f}, cube.pVertices[17].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[17].position,cube.pVertices[17].position);
    glm_vec3_copy((vec3){0.0f,1.f,0.f}, cube.pVertices[17].normal);
    glm_vec3_copy(color, cube.pVertices[17].color);
    glm_vec2_copy((vec2){1.f,0.f}, cube.pVertices[17].uv);

    
    glm_vec3_copy((vec3){-0.5f,0.5f,-0.5f}, cube.pVertices[18].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[18].position,cube.pVertices[18].position);
    glm_vec3_copy((vec3){0.f,1.f,0.f}, cube.pVertices[18].normal);
    glm_vec3_copy(color, cube.pVertices[18].color);
    glm_vec2_copy((vec2){0.f,1.f}, cube.pVertices[18].uv);

    
    glm_vec3_copy((vec3){0.5f,0.5f,-0.5f}, cube.pVertices[19].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[19].position,cube.pVertices[19].position);
    glm_vec3_copy((vec3){0.f,1.f,0.f}, cube.pVertices[19].normal);
    glm_vec3_copy(color, cube.pVertices[19].color);
    glm_vec2_copy((vec2){1.f,1.f}, cube.pVertices[19].uv);

    //Bottom Face
    glm_vec3_copy((vec3){-0.5f,-0.5f,-0.5f}, cube.pVertices[20].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[20].position,cube.pVertices[20].position);
    glm_vec3_copy((vec3){0.f,-1.f,0.f}, cube.pVertices[20].normal);
    glm_vec3_copy(color, cube.pVertices[20].color);
    glm_vec2_copy((vec2){0.f,0.f}, cube.pVertices[20].uv);

    
    glm_vec3_copy((vec3){0.5f,-0.5f,-0.5f}, cube.pVertices[21].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[21].position,cube.pVertices[21].position);
    glm_vec3_copy((vec3){0.f,-1.f,0.f}, cube.pVertices[21].normal);
    glm_vec3_copy(color, cube.pVertices[21].color);
    glm_vec2_copy((vec2){1.f,0.f}, cube.pVertices[21].uv);

    
    glm_vec3_copy((vec3){-0.5f,-0.5f,0.5f}, cube.pVertices[22].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[22].position,cube.pVertices[22].position);
    glm_vec3_copy((vec3){0.f,-1.f,0.f}, cube.pVertices[22].normal);
    glm_vec3_copy(color, cube.pVertices[22].color);
    glm_vec2_copy((vec2){0.f,1.f}, cube.pVertices[22].uv);

    
    glm_vec3_copy((vec3){0.5f,-0.5f,0.5f}, cube.pVertices[23].position);
    glm_vec3_mul((vec3){width,height,depth}, cube.pVertices[23].position,cube.pVertices[23].position);
    glm_vec3_copy((vec3){0.f,-1.f,0.f}, cube.pVertices[23].normal);
    glm_vec3_copy(color, cube.pVertices[23].color);
    glm_vec2_copy((vec2){1.f,1.f}, cube.pVertices[23].uv);


    cube.pIndices[0] = 0;
    cube.pIndices[1] = 1;
    cube.pIndices[2] = 2;
    cube.pIndices[3] = 2;
    cube.pIndices[4] = 1;
    cube.pIndices[5] = 3;
    cube.pIndices[6] = 4;
    cube.pIndices[7] = 5;
    cube.pIndices[8] = 6;
    cube.pIndices[9] = 6;
    cube.pIndices[10] = 5;
    cube.pIndices[11] = 7;
    cube.pIndices[12] = 8;
    cube.pIndices[13] = 9;
    cube.pIndices[14] = 10;
    cube.pIndices[15] = 10;
    cube.pIndices[16] = 9;
    cube.pIndices[17] = 11;
    cube.pIndices[18] = 12;
    cube.pIndices[19] = 13;
    cube.pIndices[20] = 14;
    cube.pIndices[21] = 14;
    cube.pIndices[22] = 13;
    cube.pIndices[23] = 15;
    cube.pIndices[24] = 16;
    cube.pIndices[25] = 17;
    cube.pIndices[26] = 18;
    cube.pIndices[27] = 18;
    cube.pIndices[28] = 17;
    cube.pIndices[29] = 19;
    cube.pIndices[30] = 20;
    cube.pIndices[31] = 21;
    cube.pIndices[32] = 22;
    cube.pIndices[33] = 22;
    cube.pIndices[34] = 21;
    cube.pIndices[35] = 23;

    return cube;
}
