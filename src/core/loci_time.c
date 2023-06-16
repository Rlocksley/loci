#include "loci_time.h"

float loci_minDeltaTime;
float loci_maxDeltaTime;
float loci_startTime;
float loci_time;
float loci_deltaTime;

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