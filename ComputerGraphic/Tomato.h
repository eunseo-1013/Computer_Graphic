#pragma once
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

/// 1 loadtexture 합치기  o
/// 2. main 에 페이지 번호 추가하기 
/// 3. 게이지  바 추가하기 o
/// 4. skybox 이용해서 모든 화면에 배경 추가 하기  <-  ??? 안돼
/// 5. 클릭하면 게이지 바 올라가기 
/// 6. 조작 통일하기 0
/// 7. 물통 불러오기 
/// 4  ->  5,2

// 마우스 회전 관련





extern float angleX;
extern float angleY;
extern int prevX, prevY;
extern bool isDragging;

// Quadric
extern GLUquadric* quad;

// 텍스처 전역 변수
extern GLuint gLeafTex;
extern GLuint gSoilTex;
extern GLuint gStemTex;




void SetupLighting();

void FlowerPot(float size);

void Soild();

void Stem(float r, float h, float slice = 30);

void TomatoLeaf();

void TomatoCapLeaves(float r);


void Tomato(float r);
void DrawTomatoLeaf(float len, float wid);

void TomatoDisplay(float stem_h, float angle_y, int count = 2);

void TomatoDisplay();