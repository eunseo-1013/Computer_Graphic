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
    float spoutLocalX = -0.13f;
    float spoutLocalY = 0.563f;

    float jitter = 8000.0f; // 이 값이 클수록 범위가 좁아짐
    float randomX = ((rand() % 100) - 50) / jitter;
    float randomY = ((rand() % 100) - 50) / jitter;

    // 회전 변환
    float startX = (spoutLocalX+randomX) * cos(rad) - spoutLocalY * sin(rad);
    float startY = (spoutLocalX+randomY) * sin(rad) + spoutLocalY * cos(rad);

    drops[i].x = startX;
    drops[i].y = startY;
    drops[i].z = ((rand() % 100) - 50) / 5000.0f; // 약간의 두께감

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
            if (drops[i].life < 0.0f || drops[i].y < -0.1f) {
                drops[i].active = false;
            }
        }
        else if (isPouring) {
            // 물이 나오는 중이면 일정 확률로 생성
            if ((rand() % 10) < 2) {
                RespawnDrop(i, tiltAngle);
            }
        }
    }
}

void DrawWaterParticles(float kettleX, float kettleY, float kettleZ, float tiltAngle) {
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

    // ==========================================
    // [핵심 기능] 주둥이 부분 클리핑 (Clip Plane)
    // ==========================================

    // 1. 현재 주둥이 끝의 위치 계산 (RespawnDrop과 동일한 공식)
    float rad = tiltAngle * (float)(M_PI / 180.0f);
    float spoutLocX = -0.13f;
    float spoutLocY = 0.56f;

    // 회전된 주둥이 끝 좌표 (자르는 기준점)
    float cutX = spoutLocX * cos(rad) - spoutLocY * sin(rad);
    float cutY = spoutLocX * sin(rad) + spoutLocY * cos(rad);

    // 2. 자르는 방향 (법선 벡터, Normal)
    // 물이 나가는 방향(rad + PI)으로 설정하면, 그 방향의 '앞쪽'만 남기고 '뒤쪽'은 자릅니다.
    float flowDir = rad + (float)M_PI; // 물이 흐르는 방향
    float nx = cos(flowDir);
    float ny = sin(flowDir);

    // 3. 평면 방정식 (Ax + By + Cz + D = 0)
    // D = -(Normal . Point)
    // 약간의 오차로 잘린 단면이 보일 수 있으므로, 기준점을 아주 살짝(-0.02f) 뒤로 밀어서 자연스럽게 처리
    float offset = 0.005f;
    double planeEq[] = { nx, ny, 0.0, -(nx * cutX + ny * cutY) + offset };

    // 4. 클립 플레인 활성화
    glClipPlane(GL_CLIP_PLANE0, planeEq);
    glEnable(GL_CLIP_PLANE0);

    for (int i = 0; i < MAX_DROPS; i++) {
        if (drops[i].active) {
            float alpha = 0.3f;
            glColor4f(drops[i].r, drops[i].g, drops[i].b, drops[i].life*alpha);

            float x = drops[i].x;
            float y = drops[i].y;
            float z = drops[i].z;
            
            float scale = 0.0001f;

            // [핵심 변경] 속도에 따른 스트레치 (Motion Blur)
            // 물방울이 떨어지는 속도 벡터(vx, vy)를 이용해 꼬리를 그립니다.
            float tailFactor = 2.0f; // 꼬리 길이 계수 (클수록 길어짐)
            float tailX = x - drops[i].vx * tailFactor;
            float tailY = y - drops[i].vy * tailFactor;

            // 간단한 빌보드 (항상 카메라를 봐야 하지만 여기선 Z축 평면으로 그림)
            glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(1, 1); glVertex3f(x + scale, y+scale, z);
            glTexCoord2f(0, 1); glVertex3f(x - scale, y+scale, z);
            /*glTexCoord2f(1, 0); glVertex3f(x + scale, y - scale, z);
            glTexCoord2f(0, 0); glVertex3f(x - scale, y - scale, z);*/
            glTexCoord2f(1, 0); glVertex3f(tailX + scale, tailY, z);
            glTexCoord2f(0, 0); glVertex3f(tailX - scale, tailY, z);
            glEnd();
        }
    }
    glDisable(GL_CLIP_PLANE0);
    glPopMatrix();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}