#pragma once


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Camera.h"


extern camera cam;

extern int windowWidth;
extern int windowHeight;
extern bool keys[256];
extern vec3 gKettleBasePos;

extern bool firstMouse;
extern float lastX;
extern float lastY;
extern float camYaw;
extern float camPitch;
extern float cameraSpeed;
extern float kettleAngle;
extern float waterTime;
extern float kettleLift;
extern bool kettleSelected;


void keyboard(unsigned char key, int x, int y);

void specialKeys(int key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);

bool IsKettleInCrosshairT();
bool IsKettleInCrosshair();
void mouseClickT(int button, int state, int x, int y);
void mouseClick(int button, int state, int x, int y);

void mouseMove(int x, int y);

void moveCamera();


void DrawCrosshair();

void updateWater();
void update(int value);