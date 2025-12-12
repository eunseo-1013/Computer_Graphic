#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

struct SteamParticle {
    float x, y, z;      // 위치
    float vx, vy, vz;   // 속도
    float life;         // 수명 (1.0 -> 0.0)
    float size;         // 크기
    bool active;
};

void InitSteamParticles();
void UpdateSteamParticles(float gauge);
void DrawSteamParticles();
