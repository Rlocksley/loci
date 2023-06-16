#ifndef LOCI_BUFFER_H
#define LOCI_BUFFER_H
#include "loci_buffer.h"
#endif

Loci_BufferInterface loci_eyeBuffer;

typedef struct Loci_Eye
{
    vec4 position;
    vec4 direction;
    mat4 viewInverse;
    mat4 projInverse;
    vec4 clip;
}Loci_Eye;

void loci_writeToEyeBuffer(Loci_Eye* pEye);


void loci_writeToEyeBuffer(Loci_Eye* pEye)
{
    memcpy(loci_eyeBuffer.pMemory, pEye, sizeof(Loci_Eye));
}
