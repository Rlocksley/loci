#include "loci_texture.h"

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
