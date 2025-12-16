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

    float spoutLocalX = -0.13f;
    float spoutLocalY = 0.563f;

    float jitter = 8000.0f; 
    float randomX = ((rand() % 100) - 50) / jitter;
    float randomY = ((rand() % 100) - 50) / jitter;

    float startX = (spoutLocalX+randomX) * cos(rad) - spoutLocalY * sin(rad);
    float startY = (spoutLocalX+randomY) * sin(rad) + spoutLocalY * cos(rad);

    drops[i].x = startX;
    drops[i].y = startY;
    drops[i].z = ((rand() % 100) - 50) / 5000.0f; 

    // 초기 속도 벡터 계산
    float speed = 0.05f + (float)(rand() % 10) / 500.0f;
    float flowAngle = rad + (float)(M_PI / 1.0f); 

    drops[i].vx = cos(flowAngle) * speed * 0.5f;
    drops[i].vy = sin(flowAngle) * speed * 0.5f;
    drops[i].vz = 0.0f;

    drops[i].life = 1.0f;
    drops[i].fade = (float)(rand() % 100) / 1000.0f + 0.005f;
    drops[i].active = true;

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
    float gravity = -0.005f; 

    for (int i = 0; i < MAX_DROPS; i++) {
        if (drops[i].active) {
            drops[i].x += drops[i].vx;
            drops[i].y += drops[i].vy;
            drops[i].z += drops[i].vz;

            drops[i].vy += gravity; 
            drops[i].life -= drops[i].fade;

            if (drops[i].life < 0.0f || drops[i].y < -0.1f) {
                drops[i].active = false;
            }
        }
        else if (isPouring) {
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

    glPushMatrix();
    glTranslatef(kettleX, kettleY, kettleZ);

    float rad = tiltAngle * (float)(M_PI / 180.0f);
    float spoutLocX = -0.13f;
    float spoutLocY = 0.56f;

    float cutX = spoutLocX * cos(rad) - spoutLocY * sin(rad);
    float cutY = spoutLocX * sin(rad) + spoutLocY * cos(rad);

    float flowDir = rad + (float)M_PI; // 물이 흐르는 방향
    float nx = cos(flowDir);
    float ny = sin(flowDir);

    float offset = 0.005f;
    double planeEq[] = { nx, ny, 0.0, -(nx * cutX + ny * cutY) + offset };

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

            float tailFactor = 2.0f;
            float tailX = x - drops[i].vx * tailFactor;
            float tailY = y - drops[i].vy * tailFactor;

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