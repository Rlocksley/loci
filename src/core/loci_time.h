#ifndef LOCI_TIME_H
#define LOCI_TIME_H

#include "loci_window.h"

float loci_minDeltaTime;
float loci_maxDeltaTime;
float loci_startTime;
float loci_time;
float loci_deltaTime;


void loci_createTime();
void loci_updateTime();

#endif