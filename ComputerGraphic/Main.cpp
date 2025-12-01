#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Camera.h"
#include "cup_noodle.h" 
#include "water_kettle.h"

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

    cam.InitCamera(vec3(0.0f, 1.0f, 3.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));

    for (int i = 0; i < 256; i++) keys[i] = false;

    // 컵라면 객체 텍스처 로드 
    InitCupNoodleTextures();
    // 물병 물 텍스처
    InitWaterKettleTextures();
}

void DrawCrosshair()
{
    float cx = windowWidth / 2.0f;
    float cy = windowHeight / 2.0f;
    float radius = 2.0f;   // 점 크기 (픽셀 단위)
    int segments = 32;     // 원 부드럽게

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
        float x = cx + cos(angle) * radius;
        float y = cy + sin(angle) * radius;
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
    gluPerspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z,
        cam.at.x, cam.at.y, cam.at.z,
        cam.up.x, cam.up.y, cam.up.z);
    static float t = 0.0f;
    t += 0.01f;
    waterTime = t;

    // 컵라면
    DrawCupNoodleScene();
    UpdateWaterKettle();
    // 물통
    DrawWaterKettle(0.8f, 0.05f + kettleLift, -0.2f, waterTime, kettleAngle);

    //cup_object();
    DrawCrosshair();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) glutLeaveMainLoop();
    keys[key] = true;
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        kettleAngle += 1.5f;
        if (kettleAngle > 80.0f) kettleAngle = 80.0f;
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

void mouseClick(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (x > windowWidth * 0.55f && x < windowWidth * 0.85f &&
            y > windowHeight * 0.5f && y < windowHeight * 0.9f)
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
    vec3 forward = normalize(cam.at - cam.eye);
    vec3 right = normalize(cross(forward, cam.up));

    if (keys['w']) cam.MoveCamera(forward * cameraSpeed);
    if (keys['s']) cam.MoveCamera(-forward * cameraSpeed);
    if (keys['a']) cam.MoveCamera(-right * cameraSpeed);
    if (keys['d']) cam.MoveCamera(right * cameraSpeed);
    if (keys['e']) cam.MoveCamera(cam.up * cameraSpeed);
    if (keys['c']) cam.MoveCamera(-cam.up * cameraSpeed);

    glutPostRedisplay();
    glutTimerFunc(16, moveCamera, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cup Ramen Master (1-Person View)");

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