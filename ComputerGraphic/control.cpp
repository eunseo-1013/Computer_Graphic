#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "control.h"
#include "Camera.h"
#include "gauge_circle.h"

using namespace glm;



vec3 gKettleBasePos = vec3(0.8f, -0.3f, -0.2f);
camera cam;

int windowWidth = 1280;
int windowHeight = 720;
bool keys[256] = { false };

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
    if (kettleAngle > 40.0f && kettleAngle < 50.0f) {
        g_value += 0.1f;
        if (g_value >=1.0f) g_value = 1.0f;  // 최대 1.0
    }
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}





// 물병 위치 하드코딩 수정 필요!!
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


// 토마토 용

bool IsKettleInCrosshairT() {
    vec3 rayOrigin = cam.eye;
    vec3 rayDir = normalize(cam.at - cam.eye);

    // 물통 월드 위치 (그리는 위치와 100% 동일)
    vec3 kettlePos = gKettleBasePos + vec3(0.0f, kettleLift, 0.0f);

    vec3 toKettle = kettlePos - rayOrigin;
    float proj = dot(toKettle, rayDir);
    if (proj < 0.0f) return false;   // 카메라 뒤에 있으면 탈락

    vec3 closestPoint = rayOrigin + proj * rayDir;
    float dist = length(kettlePos - closestPoint);

    return (dist < 0.3f);  // 물통 반지름 + 여유
}


//토마토용

void mouseClickT(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (IsKettleInCrosshairT())
        {
            kettleSelected = !kettleSelected;
            
           

            glutPostRedisplay();
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



void DrawCrosshair()
{
    float cx = windowWidth / 2.0f;
    float cy = windowHeight / 2.0f;
    float radius = 4.0f;   // 점 크기 (픽셀 단위)
    int segments = 32;     // 원 부드럽게
    float aspect = (float)windowHeight / (float)windowWidth;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glColor3f(0.0f, .0f, .0f); // 흰색 원

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);  // 중심
    for (int i = 0; i <= segments; i++) {
        float angle = i * 2.0f * 3.1415926f / segments;
        float x = cx + cos(angle) * radius * aspect;
        float y = cy + sin(angle) * radius * aspect;
        glVertex2f(x, y);
    }
    glEnd();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}