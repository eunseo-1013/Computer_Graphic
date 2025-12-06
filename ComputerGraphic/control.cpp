#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "control.h"
#include "Camera.h"



camera cam;

int windowWidth = 1280;
int windowHeight = 720;
bool keys[256];

bool firstMouse = true;
float lastX = windowWidth / 2.0;
float lastY = windowHeight / 2.0;
float camYaw = -90.0f;
float camPitch = 0.0f;
float cameraSpeed = 0.05f;
float kettleAngle = 0.0f;
float waterTime = 0.0f;
float kettleLift = 0.0f;
bool kettleSelected = false;


void keyboard(unsigned char key, int x, int y) {
    if (key == 27) glutLeaveMainLoop();
    keys[key] = true;
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT && kettleSelected) {
        kettleAngle += 1.5f;
        if (kettleAngle > 50.0f) kettleAngle = 50.0f;
    }

    if (key == GLUT_KEY_RIGHT) {
        kettleAngle -= 1.5f;
        if (kettleAngle < 0.0f) kettleAngle = 0.0f;
    }
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

bool IsKettleInCrosshair() {
    vec3 rayOrigin = cam.eye;
    vec3 rayDir = normalize(cam.at - cam.eye);

    vec3 kettlePos = vec3(0.8f, 8.851f + kettleLift, -0.2f);

    // 벡터 projection
    vec3 toKettle = kettlePos - rayOrigin;

    float proj = dot(toKettle, rayDir);  // 시선 방향으로 투영한 길이
    if (proj < 0) return false;          // 뒤쪽이면 false

    // 실제 레이와 물병 사이 최단 거리
    vec3 closestPoint = rayOrigin + proj * rayDir;
    float dist = length(kettlePos - closestPoint);

    return (dist < 0.3f); // 허용 오차 (물병 반지름)
}

void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (IsKettleInCrosshair())
        {
            kettleSelected = !kettleSelected; // 토글
        }
    }
}

void mouseMove(int x, int y) {
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float xoffset = x - lastX;
    float yoffset = lastY - y;
    lastX = x;
    lastY = y;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camYaw += xoffset;
    camPitch += yoffset;
    if (camPitch > 89.0f) camPitch = 89.0f;
    if (camPitch < -89.0f) camPitch = -89.0f;

    vec3 dir;
    dir.x = cos(radians(camYaw)) * cos(radians(camPitch));
    dir.y = sin(radians(camPitch));
    dir.z = sin(radians(camYaw)) * cos(radians(camPitch));

    cam.at = cam.eye + normalize(dir);
    cam.UpdateCamera();
}

void moveCamera(int value) {
    vec3 forward = vec3(0.0f, 0.0f, -1.0f);
    vec3 right = vec3(1.0f, 0.0f, 0.0f);

    if (keys['w']) cam.MoveCamera(forward * cameraSpeed);
    if (keys['s']) cam.MoveCamera(-forward * cameraSpeed);
    if (keys['a']) cam.MoveCamera(-right * cameraSpeed);
    if (keys['d']) cam.MoveCamera(right * cameraSpeed);
    if (keys['e']) cam.MoveCamera(cam.up * cameraSpeed);
    if (keys['c']) cam.MoveCamera(-cam.up * cameraSpeed);

    glutPostRedisplay();
    glutTimerFunc(16, moveCamera, 0);
}
