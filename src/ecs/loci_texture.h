#ifndef LOCI_IMAGE_H
#define LOCI_IMAGE_H
#include "../core/loci_image.h"
#endif
#ifndef LOCI_WORLD_H
#define LOCI_WORLD_H
#include "../core/loci_world.h"
#endif

typedef struct Loci_Texture
{
    Loci_Image image;
}Loci_Texture;


void loci_createTexture(ecs_entity_t entity, const char* pFilePath);
void loci_destroyTexture(ecs_entity_t entity);

void loci_createTexture(ecs_entity_t entity, const char* pFilePath)
{
    Loci_Texture texture;
    texture.image = loci_createImage(pFilePath);

    loci_set(entity, Loci_Texture, texture);
}

void loci_destroyTexture(ecs_entity_t entity)
{
    const Loci_Texture* pTexture;
    loci_get(entity, Loci_Texture, pTexture);

    loci_destroyImage(pTexture->image);

    loci_remove(entity, Loci_Texture);
}
