#include "loci_eye.h"

Loci_BufferInterface loci_eyeBuffer;

void loci_writeToEyeBuffer(Loci_Eye* pEye)
{
    memcpy(loci_eyeBuffer.pMemory, pEye, sizeof(Loci_Eye));
}
