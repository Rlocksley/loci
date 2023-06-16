#ifndef LOCI_TIME_H
#define LOCI_TIME_H

#include "loci_window.h"

extern float loci_minDeltaTime;
extern float loci_maxDeltaTime;
extern float loci_startTime;
extern float loci_time;
extern float loci_deltaTime;


void loci_createTime();
void loci_updateTime();

#endif