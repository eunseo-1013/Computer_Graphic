#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

extern float waterTime;
extern float kettleLift;
extern bool kettleSelected;

void DrawWaterKettle(float x, float y, float z, float time, float tiltAngle);
void InitWaterKettleTextures();
void UpdateWaterKettle();