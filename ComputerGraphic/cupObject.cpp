#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
//#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
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


// 마우스 회전 관련 전역 변수
float angleX = 0.0f;
float angleY = 0.0f;
int prevX, prevY;
bool isDragging = false;

// GLU Quadric 객체 선언
GLUquadric* quad = NULL;

// 조명 및 유리 재질 초기화
void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    // --------------------------
    // 조명 설정
    // --------------------------
    GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { 2.0f, 3.0f, 3.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    // --------------------------
    // 유리 재질 설정
    // --------------------------
    GLfloat matAmbient[] = { 0.1f, 0.1f, 0.1f, 0.3f };
    GLfloat matDiffuse[] = { 0.6f, 0.8f, 0.9f, 0.3f }; // 살짝 푸른빛
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 0.3f };
    GLfloat matShininess[] = { 100.0f }; // 반짝임 강함

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // 법선 자동 계산
    if (quad) gluQuadricNormals(quad, GLU_SMOOTH);
}

// --------------------------
// 컵 그리기
// --------------------------
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

// --------------------------
// 디스플레이 콜백
// --------------------------
/*
void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    // 마우스 회전 적용
    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    glPushMatrix();
    glTranslatef(0, -1.0, 0); // 컵 위치 조정
    cup_object();
    glPopMatrix();

    glutSwapBuffers();
}

void MyDisplay(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /*
    gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z,
        cam.at.x, cam.at.y, cam.at.z,
        cam.up.x, cam.up.y, cam.up.z);
        
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    glPushMatrix();
    glTranslatef(0, -1.0, 0); // 컵 위치 조정
    cup_object();
    glPopMatrix();

    glutSwapBuffers();
}


// --------------------------
// 리쉐이프 콜백
// --------------------------
void MyReshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 100);
}

// --------------------------
// 마우스
// --------------------------
/*
void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            prevX = x;
            prevY = y;
        }
        else {
            isDragging = false;
           
        }
    }

 
}

void Motion(int x, int y) {
    if (!isDragging) {
        int dx = x - prevX;
        int dy = y - prevY;

        angleY += dx * 0.2f;
        angleX += dy * 0.2f;

        if (angleX > 360.0f) angleX -= 360.0f;
        if (angleX < -360.0f) angleX += 360.0f;
        if (angleY > 360.0f) angleY -= 360.0f;
        if (angleY < -360.0f) angleY += 360.0f;

        prevX = x;
        prevY = y;
        glutPostRedisplay();
    }
}
*/
/*

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

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) glutLeaveMainLoop();
    keys[key] = true;
}

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

// --------------------------
// 메인
// --------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Transparent Glass Cup with Rotation");

    quad = gluNewQuadric();

    // 배경 어둡게
    glClearColor(1,1 , 1, 1);

    initLighting();

    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    //glutMouseFunc(Mouse);
    //glutMotionFunc(Motion);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutPassiveMotionFunc(mouseMove);
    glutTimerFunc(16, moveCamera, 0);

    glutMainLoop();

    if (quad) gluDeleteQuadric(quad);
    return 0;
}


*/