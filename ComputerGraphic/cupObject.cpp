#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>


/*
using namespace std;



int windowWidth = 1280;
int windowHeight = 720;

float angleX = 0.0f;
float angleY = 0.0f;
int   prevX, prevY;
bool  isDragging = false;

GLUquadric* quad = NULL;

void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    // 조명 설정
    GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { 2.0f, 3.0f, 3.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


   // 유리 재질 설정
    GLfloat matAmbient[] = { 0.1f, 0.1f, 0.1f, 0.3f };
    GLfloat matDiffuse[] = { 0.7f, 0.9f, 1.0f, 0.6f }; // 푸른빛
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 0.3f };
    GLfloat matShininess[] = { 100.0f };             // 반짝임 추가

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
}

void cup_object() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    glRotatef(-90, 1, 0, 0); // 컵을 위로 세우기



    float outerBottom = 0.5f;
    float outerTop = 0.8f;
    float height = 2.0f;

    float thickness = 0.05f;          // 유리 두께
    float innerBottom = outerBottom - thickness;
    float innerTop = outerTop - thickness;

    int slices = 40;


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);


    gluCylinder(quad, outerBottom, outerTop, height, slices, slices);

    gluDisk(quad, 0.0f, outerBottom, slices, 1);

  
    glPushMatrix();
    glTranslatef(0, 0, height);
    glRotatef(180, 1, 0, 0); // 위에서 아래로 뒤집기
    gluCylinder(quad, innerTop, innerBottom, height-0.2, slices, slices);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, height); // 컵 윗부분 z = height 위치
    gluDisk(quad, innerTop, outerTop, slices, 1);
    glPopMatrix();

  
    gluDisk(quad, innerBottom, outerBottom, slices, 1);

 
    glPushMatrix();
    glTranslatef(0, 0, 0.01f); // Z-fighting 방지용 약간 올림
    gluDisk(quad, 0.0f, innerBottom, slices, 1);
    glPopMatrix();

   
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glPopMatrix();
    gluDeleteQuadric(quad);
}



void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5,   // eye
        0, 0, 0,   // center
        0, 1, 0);  // up

    // 마우스 회전 적용
    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    glPushMatrix();
    glTranslatef(0, -1.0, 0); // 컵 위치 조정
    cup_object();
    glPopMatrix();

    glutSwapBuffers();
}

void MyDisplay_Old(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,
                   (float)windowWidth / (float)windowHeight,
                   0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5,
              0, 0, 0,
              0, 1, 0);

    glPushMatrix();
    glTranslatef(0, -1.0, 0);
    cup_object();
    glPopMatrix();

    glutSwapBuffers();
}
*/



/*

void MyReshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,
        (GLfloat)w / (GLfloat)h,
        0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}


void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            prevX = x;
            prevY = y;
        }
        else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
}

void Motion(int x, int y) {
    if (isDragging) {
        int dx = x - prevX;
        int dy = y - prevY;

        angleY += dx * 0.3f;
        angleX += dy * 0.3f;

        // 각도 값 너무 커지지 않게 간단히 제한
        if (angleX > 360.0f)  angleX -= 360.0f;
        if (angleX < -360.0f) angleX += 360.0f;
        if (angleY > 360.0f)  angleY -= 360.0f;
        if (angleY < -360.0f) angleY += 360.0f;

        prevX = x;
        prevY = y;

        glutPostRedisplay();
    }
}


void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC
        glutLeaveMainLoop();
    }
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Transparent Glass Cup with Rotation");

    // GLEW 초기화 (필수 환경이면)
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        cerr << "Error: " << glewGetErrorString(err) << endl;
        return -1;
    }

    // 배경 흰색
    glClearColor(1.0, 1.0, 1.0, 1.0);

    initLighting();

    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    if (quad) gluDeleteQuadric(quad);
    return 0;
}
*/