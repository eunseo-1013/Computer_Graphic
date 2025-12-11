#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

extern float waterTime;
extern float kettleLift;
extern bool kettleSelected;
extern float kettleAngle;


extern float waterY;     // 물이 떨어지는 y 위치
extern bool isPouring ;  // 기울여서 물이 나오는 상태

void DrawWaterKettle(float x, float y, float z, float time, float tiltAngle);
void InitWaterKettleTextures();
void UpdateWaterKettle();