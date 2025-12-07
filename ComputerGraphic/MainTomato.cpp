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
#include "Tomato.h"



//------------------------------
// 기타 콜백
//------------------------------
void MyReshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLfloat)w / (GLfloat)h, 0.1, 100);
}




/*
void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            prevX = x;
            prevY = y;
            g_value += 0.1f;
            if (g_value > 1.0f) g_value = 1.0f;  // 최대 1.0

            glutPostRedisplay();
        }
        else {
            isDragging = false;
        }
    }
}

void Motion(int x, int y) {
    if (isDragging) {
        int dx = x - prevX;
        int dy = y - prevY;

        angleY += dx * 0.5f;
        angleX += dy * 0.5f;

        prevX = x;
        prevY = y;

        glutPostRedisplay();
    }
}


*/


bool InitTextures()
{
    gLeafTex = gLoadTexture("texture/24leaf_texture.bmp");
    gSoilTex = gLoadTexture("texture/soild_texture.bmp");
    gStemTex = gLoadTexture("texture/stem_texture.bmp");

    if (!gLeafTex || !gSoilTex || !gStemTex) {
        printf("텍스처 로드 실패\n");
        return false;
    }
    return true;
}


//------------------------------
// main
//------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Cherry Tomato Plant (Spiral)");


    InitSkybox();
    // ★ 카메라 초기 세팅
    cam.eye = vec3(0.0f, 3.0f, 3.0f);
    cam.at = vec3(0.0f, 0.5f, 0.0f);
    cam.up = vec3(0.0f, 1.0f, 0.0f);
    cam.UpdateCamera();   // 이런 함수가 있다면 호출



    glShadeModel(GL_SMOOTH);
    SetupLighting();

    glEnable(GL_DEPTH_TEST);

    if (!InitTextures()) {
        return -1;
    }


    //LoadTomatoTexture("C:/Users/eunse/source/repos/ComputerGraphic/tomato_texture.bmp");


    glClearColor(1, 1, 1, 1);

    if (!quad) quad = gluNewQuadric();

    glutDisplayFunc(TomatoDisplay);

    glutReshapeFunc(MyReshape);
    /*
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);*/


    //glutReshapeFunc(reshape);
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