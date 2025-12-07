#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Camera.h"
#include "cup_noodle.h" 
#include "water_kettle.h"
#include "skybox.h"
#include "control.h"




/*
camera cam;
using namespace glm;
using namespace std;

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

*/

void cup_object() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);

    // --------------------------
    // 투명 유리 효과
    // --------------------------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    // 컵 외벽 (잘린 원뿔)
    gluCylinder(quad, 0.5, 0.8, 2.0, 30, 30);

    // 컵 바닥
    gluDisk(quad, 0.0, 0.5, 30, 1);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glPopMatrix();
    gluDeleteQuadric(quad);
}

void init() {
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat back_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat back_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_BACK, GL_AMBIENT, back_ambient);
    glMaterialfv(GL_BACK, GL_DIFFUSE, back_diffuse);

    glEnable(GL_NORMALIZE);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    cam.InitCamera(vec3(0.0f, 10.0f, 3.0f),
        vec3(0.0f, 10.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));

    for (int i = 0; i < 256; i++) keys[i] = false;

    // 컵라면 객체 텍스처 로드 
    InitCupNoodleTextures();
    // 물병 물 텍스처
    InitWaterKettleTextures();
    // 배경
    InitSkybox();
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

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z,
        cam.at.x, cam.at.y, cam.at.z,
        cam.up.x, cam.up.y, cam.up.z);
    static float t = 0.0f;
    t += 0.01f;
    waterTime = t;

    // 배경
    DrawSkybox();

    // 컵라면
    DrawCupNoodleScene();
    UpdateWaterKettle();
    // 물통
    DrawWaterKettle(0.8f, 8.851f + kettleLift, -0.2f, waterTime, kettleAngle);

    //cup_object();
    DrawCrosshair();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}
/*
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) glutLeaveMainLoop();
    keys[key] = true;
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT && kettleSelected) {
        kettleAngle += 1.5f;
        if (kettleAngle > 50.0f) kettleAngle = 50.0f;
    }

    if (key == GLUT_KEY_RIGHT && kettleSelected) {
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
    // 1) 현재 카메라의 바라보는 방향(front)을 계산
    vec3 front = normalize(cam.at - cam.eye);

    // 2) 오른쪽 방향
    vec3 right = normalize(cross(front, cam.up));

    if (keys['w']) cam.MoveCamera(front * cameraSpeed);
    if (keys['s']) cam.MoveCamera(-front * cameraSpeed);
    if (keys['a']) cam.MoveCamera(-right * cameraSpeed);
    if (keys['d']) cam.MoveCamera(right * cameraSpeed);
    if (keys['e']) cam.MoveCamera(cam.up * cameraSpeed);
    if (keys['c']) cam.MoveCamera(-cam.up * cameraSpeed);

    glutPostRedisplay();
    glutTimerFunc(16, moveCamera, 0);
}


*/


int main3(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cup Ramen Master (1-Person View)");

    glutFullScreen();
    glewInit();
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mouseMove);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutTimerFunc(16, moveCamera, 0);

    glutMainLoop();
    return 0;
}



