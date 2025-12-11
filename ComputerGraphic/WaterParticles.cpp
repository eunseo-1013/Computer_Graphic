#include "WaterParticles.h"
#include <cstdlib>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define MAX_DROPS 3000
static WaterParticle drops[MAX_DROPS];
static GLuint particleTextureID = 0;

void LoadParticleTexture() {
    AUX_RGBImageRec* pTextureImage = auxDIBImageLoad("texture/water3.bmp");
    if (pTextureImage != NULL) {
        glGenTextures(1, &particleTextureID);
        glBindTexture(GL_TEXTURE_2D, particleTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, pTextureImage->sizeX, pTextureImage->sizeY, 0,
            GL_RGB, GL_UNSIGNED_BYTE, pTextureImage->data);
    }
    if (pTextureImage) {
        if (pTextureImage->data) free(pTextureImage->data);
        free(pTextureImage);
    }
}

void RespawnDrop(int i, float tiltAngle) {
    float rad = tiltAngle * (float)(M_PI / 180.0f);

    // 주전자 주둥이 끝의 로컬 좌표 (주전자 모델링에 맞춰 미세 조정 필요)
    float spoutLocalX = -0.14f;
    float spoutLocalY = 0.553f;

    // 회전 변환
    float startX = spoutLocalX * cos(rad) - spoutLocalY * sin(rad);
    float startY = spoutLocalX * sin(rad) + spoutLocalY * cos(rad);

    drops[i].x = startX;
    drops[i].y = startY;
    drops[i].z = ((rand() % 100) - 50) / 10000.0f; // 약간의 두께감

    // 초기 속도 벡터 계산
    float speed = 0.05f + (float)(rand() % 10) / 500.0f;
    float flowAngle = rad + (float)(M_PI / 1.0f); // 뿜어져 나가는 각도

    drops[i].vx = cos(flowAngle) * speed * 0.5f;
    drops[i].vy = sin(flowAngle) * speed * 0.5f;
    drops[i].vz = 0.0f;

    drops[i].life = 1.0f;
    drops[i].fade = (float)(rand() % 100) / 1000.0f + 0.005f;
    drops[i].active = true;

    // 파란색 계열
    drops[i].r = 0.9f;
    drops[i].g = 0.9f;
    drops[i].b = 1.0f;
}

void InitWaterParticles() {
    LoadParticleTexture();
    for (int i = 0; i < MAX_DROPS; i++) {
        drops[i].active = false;
    }
}

void UpdateWaterParticles(float tiltAngle, bool isPouring) {
    float gravity = -0.005f; // 중력

    for (int i = 0; i < MAX_DROPS; i++) {
        if (drops[i].active) {
            drops[i].x += drops[i].vx;
            drops[i].y += drops[i].vy;
            drops[i].z += drops[i].vz;

            drops[i].vy += gravity; // 아래로 가속
            drops[i].life -= drops[i].fade;

            // 화면 아래로 떨어지거나 수명이 다하면
            if (drops[i].life < 0.0f || drops[i].y < -3.0f) {
                drops[i].active = false;
            }
        }
        else if (isPouring) {
            // 물이 나오는 중이면 일정 확률로 생성
            if ((rand() % 10) < 4) {
                RespawnDrop(i, tiltAngle);
            }
        }
    }
}

void DrawWaterParticles(float kettleX, float kettleY, float kettleZ) {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, particleTextureID);

    // 파티클은 월드 좌표계에서 독자적으로 움직이므로
    // 주전자 위치만큼만 보정해주고 회전은 적용하지 않습니다 (물은 주전자를 떠났으므로)
    glPushMatrix();
    glTranslatef(kettleX, kettleY, kettleZ);

    for (int i = 0; i < MAX_DROPS; i++) {
        if (drops[i].active) {
            float alpha = 0.05f;
            glColor4f(drops[i].r, drops[i].g, drops[i].b, drops[i].life*alpha);

            float x = drops[i].x;
            float y = drops[i].y;
            float z = drops[i].z;
            float scale = 0.007f;

            // 간단한 빌보드 (항상 카메라를 봐야 하지만 여기선 Z축 평면으로 그림)
            glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(1, 1); glVertex3f(x + scale, y + scale, z);
            glTexCoord2f(0, 1); glVertex3f(x - scale, y + scale, z);
            glTexCoord2f(1, 0); glVertex3f(x + scale, y - scale, z);
            glTexCoord2f(0, 0); glVertex3f(x - scale, y - scale, z);
            glEnd();
        }
    }
    glPopMatrix();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}