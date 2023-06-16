#ifndef LOCI_TEXTURE_H
#define LOCI_TEXTURE_H

#include "../core/loci_image.h"
#include "../core/loci_world.h"

typedef struct Loci_Texture
{
    Loci_Image image;
}Loci_Texture;


void loci_createTexture(ecs_entity_t entity, const char* pFilePath);
void loci_destroyTexture(ecs_entity_t entity);

#endif