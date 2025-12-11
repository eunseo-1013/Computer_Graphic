#pragma once

#include <cmath>
#include <vector>
#include "glaux.h"

// 파티클 구조체
struct WaterParticle {
    bool active;
    float life;
    float fade;
    float x, y, z;
    float vx, vy, vz;
    float r, g, b;
};

void InitWaterParticles();
void UpdateWaterParticles(float tiltAngle, bool isPouring);
void DrawWaterParticles(float kettleX, float kettleY, float kettleZ);
