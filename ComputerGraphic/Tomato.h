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


//------------------------------
// 디스플레이
//------------------------------
void TomatoDisplay();