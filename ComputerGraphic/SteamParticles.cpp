#include "SteamParticles.h"
#include "glaux.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

#define MAX_STEAM 150 // 김 파티클 개수 (너무 많으면 지저분함)

static SteamParticle steam[MAX_STEAM];
static GLuint steamTextureID = 0;

// 텍스처 로드 
void LoadSteamTexture() {
    AUX_RGBImageRec* pTextureImage = auxDIBImageLoad("texture/steam.bmp");
    if (pTextureImage != NULL) {
        glGenTextures(1, &steamTextureID);
        glBindTexture(GL_TEXTURE_2D, steamTextureID);
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

// 김이 생성되는 위치 (수면 위)
void RespawnSteam(int i, float gauge) {
    if (gauge <= 0.01f) { // 물이 없으면 생성 안 함
        steam[i].active = false;
        return;
    }

    // 컵라면 컵 내부 반지름 범위 내에서 랜덤 위치
    float angle = (rand() % 360) * (3.14159f / 180.0f);
    float radius = (rand() % 100) / 100.0f * 0.2f; // 컵 반지름(약 0.3)보다 작게

    // 중심 기준 좌표
    steam[i].x = cos(angle) * radius;
    steam[i].z = sin(angle) * radius - 0.15f;

  
    // 약간 수면보다 위에서 시작하게끔 조정
    float waterBaseY = 8.8f + 0.25f; // 컵 바닥 기준점
    steam[i].y = waterBaseY + (0.5f * gauge);
    
    // 천천히 올라오게끔 조정!
    steam[i].vy = 0.001f + (float)(rand() % 10) / 3000.0f;
    steam[i].vx = ((rand() % 100) - 50) / 20000.0f; 
    steam[i].vz = ((rand() % 100) - 50) / 20000.0f;

    steam[i].life = 1.0f;
    steam[i].size = 0.03f; // 초기 크기
    steam[i].active = true;
}

void InitSteamParticles() {
    LoadSteamTexture();
    for (int i = 0; i < MAX_STEAM; i++) {
        steam[i].active = false;
    }
}

void UpdateSteamParticles(float gauge) {
    for (int i = 0; i < MAX_STEAM; i++) {
        if (steam[i].active) {
            // 위로 이동
            steam[i].x += steam[i].vx;
            steam[i].y += steam[i].vy;
            steam[i].z += steam[i].vz;

            // 확산
            steam[i].size += 0.00015f;
            
            steam[i].life -= 0.006f;

            if (steam[i].life < 0.0f) {
                // 물이 차 있을 때만 재생성
                if (gauge > 0.05f) RespawnSteam(i, gauge);
                else steam[i].active = false;
            }
        }
        else {
            // 비활성 상태면 랜덤하게 생성
            if (gauge > 0.05f && (rand() % 50) < 2) {
                RespawnSteam(i, gauge);
            }
        }
    }
}

void DrawSteamParticles() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // 겹칠수록 하얗게 빛나야 진짜 같음
    glDepthMask(GL_FALSE); // 투명함을 위함~

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, steamTextureID);

    

    for (int i = 0; i < MAX_STEAM; i++) {
        if (steam[i].active) {
            // 투명도는 수명에 비례  !!!!!!!!!!!
            glColor4f(0.8f, 0.8f, 0.8f, steam[i].life * 0.15f);

            float x = steam[i].x;
            float y = steam[i].y;
            float z = steam[i].z;
            float s = steam[i].size;

            
            // 정면
            glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(1, 1); glVertex3f(x + s, y + s, z);
            glTexCoord2f(0, 1); glVertex3f(x - s, y + s, z);
            glTexCoord2f(1, 0); glVertex3f(x + s, y - s, z);
            glTexCoord2f(0, 0); glVertex3f(x - s, y - s, z);
            glEnd();

            //측면
            glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(1, 1); glVertex3f(x, y + s, z + s);
            glTexCoord2f(0, 1); glVertex3f(x, y + s, z - s);
            glTexCoord2f(1, 0); glVertex3f(x, y - s, z + s);
            glTexCoord2f(0, 0); glVertex3f(x, y - s, z - s);
            glEnd();
        }
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopAttrib();
}