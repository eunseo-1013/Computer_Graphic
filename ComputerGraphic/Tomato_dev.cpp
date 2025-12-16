#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include "glaux.h"
#include "loadTexture.h"
#include "gauge_circle.h"
#include "skybox.h"
#include "Camera.h"
#include "control.h"
#include "water_kettle.h"


/// 1 loadtexture 합치기  o
/// 2. main 에 페이지 번호 추가하기 
/// 3. 게이지  바 추가하기 o
/// 4. skybox 이용해서 모든 화면에 배경 추가 하기  <-  ??? 안돼
/// 5. 클릭하면 게이지 바 올라가기  o
/// 6. 조작 통일하기 0
/// 7. 물통 불러오기 
/// 4  ->  2



// 텍스처 전역 변수
GLuint gLeafTex = 0; // 잎 텍스처 번호
GLuint gSoilTex = 0; // 흙 텍스처 번호
GLuint gStemTex = 0;


// 조명 설정
void SetupLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);          // 스케일 변형해도 노멀 자동 보정
    glEnable(GL_COLOR_MATERIAL);     // glColor로 재질 색 조절
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // 화면 전체 기본 밝기
    GLfloat globalAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    // 태양 느낌의 따뜻한 흰빛
    GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 0.98f, 0.90f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 0.95f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}



// 마우스 회전 관련
float angleX = 0.0f;
float angleY = 0.0f;
int prevX, prevY;
bool isDragging = false;

// GLU Quadric
GLUquadric* quad = NULL;

// 1. 화분 및 흙

void FlowerPot(float size) {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_TEXTURE_2D);

    GLfloat oldAmb[4], oldDiff[4], oldSpec[4];
    glGetLightfv(GL_LIGHT0, GL_AMBIENT, oldAmb);
    glGetLightfv(GL_LIGHT0, GL_DIFFUSE, oldDiff);
    glGetLightfv(GL_LIGHT0, GL_SPECULAR, oldSpec);

    GLfloat potAmb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat potDiff[] = { 0.9f, 0.9f, 0.9f, 1.0f }; // 살짝 줄임
    GLfloat potSpec[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, potAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, potDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, potSpec);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    //GLfloat matAmb[] = { 0.30f, 0.18f, 0.10f, 1.0f }; // 어두운 갈색
    //GLfloat matDiff[] = { 0.50f, 0.30f, 0.16f, 1.0f }; // 본 색
    //GLfloat matSpec[] = { 0.0f, 0.0f, 0.0f, 1.0f };    // 반짝임 없음
    //GLfloat matShine[] = { 1.0f };

    //glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    //glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
    GLdouble plane[] = { 0.0, 0.0, -1, 0.7 };
    glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, plane);
    glColor3f(0.7f, 0.4f, 0.2f); // 밝은 화분 갈색
    glutSolidCone(size, size + 1, 30, 30);
    glDisable(GL_CLIP_PLANE0);
    if (!quad)
        quad = gluNewQuadric();

    glPushMatrix();
    // glColor3f(0.6f, 0.3f, 0.2f);
    // SolidCone 높이 = size + 1
    float topZ = (size) * 0.10f; // 클립된 상단 실제 위치에 맞춰 보정

    glTranslatef(0, 0, -topZ);

    float rimH = 0.15f;
    float rimOuter = size * 1.0f;
    float rimInner = size * 0.01f;

    // Outer rim
    gluCylinder(quad, rimOuter, rimOuter, rimH, 30, 1);

    // Inner rim
    gluCylinder(quad, rimInner, rimInner, rimH, 30, 1);

    glPopMatrix();


     // 아래쪽 링

    glPushMatrix();
    //glColor3f(0.6f, 0.3f, 0.2f);
    topZ = (size) * 0.67f;

    glTranslatef(0, 0, topZ*2);

     rimH = 0.1f;
     rimOuter = size * 0.55f;
     rimInner = size * 0.01f;

    // Outer rim
    gluCylinder(quad, rimOuter, rimOuter, rimH, 30, 1);

    // Inner rim
    gluCylinder(quad, rimInner, rimInner, rimH, 30, 1);

    glPopMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1.0f, 1.0f, 1.0f); // 다음 물체에 색 안 묻도록

    glLightfv(GL_LIGHT0, GL_AMBIENT, oldAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, oldDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, oldSpec);

    glPopAttrib();
}

void Soild() {
    // 텍스처 없으면 색만
    if (gSoilTex == 0) {
        GLdouble plane[] = { 0.0, 0.0, -1, 0.9 };
        glEnable(GL_CLIP_PLANE0);
        glClipPlane(GL_CLIP_PLANE0, plane);
        glColor3f(0.55f, 0.35f, 0.18f); // 조금 밝은 흙색
        glutSolidCone(0.4, 0.1, 30, 30);
        glDisable(GL_CLIP_PLANE0);
        return;
    }

    // 게이지가 올라갈수록 factor가 작아져서 어두워짐
    float factor = 1.0f - 0.5f * g_value;        // g=0 -> 1.0, g=1 -> 0.3
    if (factor < 0.0f) factor = 0.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gSoilTex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(factor, factor, factor);
    float radius = 0.40f;
    int   slices = 40;
    float z = 0.01f;

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, 0.0f, z);

    for (int i = 0; i <= slices; ++i) {
        float theta = (float)i / (float)slices * 2.0f * 3.14159265f;
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        float u = 0.5f + 0.5f * cosf(theta);
        float v = 0.5f + 0.5f * sinf(theta);

        glTexCoord2f(u, v);
        glVertex3f(x, y, z);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// 줄기 및 잎
void Stem(float r, float h, float slice = 30) {
    if (!quad) {
        quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH);   // 조명용 노멀
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gStemTex);    // 줄기 텍스처
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    gluQuadricTexture(quad, GL_TRUE);

    glColor3f(0.0f, 0.58f, 0.0f); // 줄기 기본 색
    gluQuadricDrawStyle(quad, GLU_FILL);

    gluCylinder(quad, r, r-0.025, h, slice, 1);

    // 텍스처 행렬 복구
    glMatrixMode(GL_TEXTURE);
    //glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    gluQuadricTexture(quad, GL_FALSE);
    glDisable(GL_TEXTURE_2D);
}


// 토마토 꼭대기 작은 잎
void TomatoLeaf() {
    glRotatef(90, -1, 0, 0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gLeafTex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor3f(0.0f, 0.32f, 0.0f);

    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.0f, 1.0f);

    glTexCoord2f(0.05f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.005f);

    glTexCoord2f(0.10f, 0.0f);
    glVertex3f(0.03f, 0.0f, 0.005f);

    glTexCoord2f(0.075f, 0.10f);
    glVertex3f(0.005f, 0.05f, 0.005f);

    glEnd();

    glDisable(GL_TEXTURE_2D);  
}

void TomatoCapLeaves(float r) {
    glTranslatef(0.0f, r + 0.01f, 0.0f);
    for (int i = 0; i < 3; ++i) {
        glPushMatrix();
        glRotatef(i * 120.0f, 0, 1, 0);
        TomatoLeaf();
        glPopMatrix();
    }
}

// 토마토 본체
void Tomato(float r) {
    glPushMatrix();

    glDisable(GL_TEXTURE_2D);

    glDisable(GL_COLOR_MATERIAL);
    GLfloat matAmbient[] = { 0.35f, 0.05f, 0.05f, 1.0f };  
    GLfloat matDiffuse[] = { 0.85f, 0.18f, 0.18f, 1.0f };  
    GLfloat matSpecular[] = { 0.5f, 0.6f, 0.5f, 1.0f };     
    GLfloat matShininess[] = { 35.0f };

    GLfloat matEmission[] = { 0.2f, 0.03f, 0.03f, 1.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);

    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(r, 30, 30);

    // emission 원상복구
    GLfloat zeroEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, zeroEmission);

    // 꼭대기 잎은 텍스처 사용
    glEnable(GL_TEXTURE_2D);
    TomatoCapLeaves(r);

    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPopMatrix();
}

// 큰 잎
void DrawTomatoLeaf(float len, float wid) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gLeafTex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor3f(1.0f, 1.0f, 1.0f);

    float length = len;
    float width = wid;
    float z_bulge = 0.05;
    glNormal3f(0.0f, 0.0f, 1.0f);

    // 왼쪽
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0.05f, 0.0f);  glVertex3f(0, 0, 0);
    glTexCoord2f(0.025f, 0.0f);  glVertex3f(-width / 4, 0, 0);

    glTexCoord2f(0.05f, 0.04f); glVertex3f(0, length * 0.4f, z_bulge);
    glTexCoord2f(0.0f, 0.04f); glVertex3f(-width / 2, length * 0.4f, 0);

    glTexCoord2f(0.05f, 0.08f); glVertex3f(0, length * 0.8f, z_bulge / 2);
    glTexCoord2f(0.025f, 0.08f); glVertex3f(-width / 4, length * 0.8f, 0);

    glTexCoord2f(0.05f, 0.1f);  glVertex3f(0, length, 0);
    glTexCoord2f(0.05f, 0.1f);  glVertex3f(0, length, 0);
    glEnd();

    // 오른쪽
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0.05f, 0.0f);  glVertex3f(0, 0, 0);
    glTexCoord2f(0.075f, 0.0f);  glVertex3f(width / 4, 0, 0);

    glTexCoord2f(0.05f, 0.04f); glVertex3f(0, length * 0.4f, z_bulge);
    glTexCoord2f(0.1f, 0.04f); glVertex3f(width / 2, length * 0.4f, 0);

    glTexCoord2f(0.05f, 0.08f); glVertex3f(0, length * 0.8f, z_bulge / 2);
    glTexCoord2f(0.075f, 0.08f); glVertex3f(width / 4, length * 0.8f, 0);

    glTexCoord2f(0.05f, 0.1f);  glVertex3f(0, length, 0);
    glTexCoord2f(0.05f, 0.1f);  glVertex3f(0, length, 0);
    glEnd();

    // 중앙 잎맥
    glColor3f(0.0f, 0.4f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, length, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);  
}

// 토마토 + 가지 묶음
void TomatoDisplay(float stem_h, float angle_y, int count = 2) {
    glPushMatrix();

    glTranslatef(0.0f, stem_h, 0.0f);
    glRotatef(angle_y, 0.0f, 1.0f, 0.0f);

    float branch_start_offset = 0.01f;
    glTranslatef(0.0f, 0.2f, branch_start_offset);

    // 잎 묶음
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(0.02f, 0, -0.02f);

    glPushMatrix();
    DrawTomatoLeaf(0.13f, 0.1f);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-60.0f, 0.0f, 0.0f, 1.0f);
    DrawTomatoLeaf(0.15f, 0.13f);
    glPopMatrix();

    glPopMatrix();

    glRotatef(20, 1.0f, 0.0f, 0.0f);

    // 가지 줄기
    float branch_length = 0.18f;
    glPushMatrix();
    glColor3f(0.0f, 0.28f, 0.0f);

    if (!quad) quad = gluNewQuadric();
    gluCylinder(quad, 0.012, 0.005, branch_length, 10, 1);
    glPopMatrix();

    // 토마토 열매
    glPushMatrix();
    glTranslatef(0.0f, -0.1f, branch_length);
    glRotatef(-40, 1, 0, 0);
    glRotatef(30, 1.0f, 0.0f, 0.0f);

    glPushMatrix();
    Tomato(0.11f);
    glPopMatrix();

    if (count >= 2) {
        glPushMatrix();
        glTranslatef(0.08f, -0.05f, 0.08f);
        glRotatef(15, 0, 1, 0);
        Tomato(0.09f);
        glPopMatrix();
    }

    glPopMatrix();
    glPopMatrix();
}






