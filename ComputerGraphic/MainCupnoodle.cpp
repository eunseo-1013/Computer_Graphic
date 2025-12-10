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
#include "gauge_circle.h"



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
  
    DrawWaterKettle(0.8f, 8.851f + kettleLift, -0.2f, waterTime, kettleAngle);
    //물: 컵이랑 같은 위치(0, 8.8, 0)로 이동 후 그리기
    //float gauge = 0.0; // 또는 0.8f 테스트
    glPushMatrix();
    glTranslatef(0.0f, 8.8f, 0.0f);
    waterCupNoodle(g_value);   // ← 여기!
    glPopMatrix();


    //cup_object();
    DrawCrosshair();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}


int CupNoodleMain(int argc, char** argv) {
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



