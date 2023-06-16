#include "loci_time.h"


void loci_createTime()
{
    loci_startTime = glfwGetTime();
    loci_time = loci_startTime;
    loci_deltaTime = loci_minDeltaTime;
}

void loci_updateTime()
{
    float lastTime = loci_time;
    loci_time = glfwGetTime();
    loci_deltaTime = loci_time - lastTime;
}