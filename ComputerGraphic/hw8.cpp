#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


/*
#include "glaux.h"



using namespace glm;

vec3 eye(0.0, 2.0, 4.0);
vec3 at(0.2, 0.0, 0.0);
vec3 x(1.0, 0.0, 0.0);
vec3 forward = normalize(eye - at);
vec3 right = normalize(cross(forward, x));
vec3 up = cross(right, forward);

static int Day = 0, Time = 0;

// ------------------------ Light ------------------------
void Light_Sun() {
    GLfloat amb[] = { 0.5, 0.0, 0.0, 1.0 };
    GLfloat diff[] = { 1.0, 0.5, 0.5, 1.0 };
    GLfloat spec[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat pos[] = { 0.0, 0.0, 0.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
}

void Light_Moon() {
    GLfloat amb[] = { 0.5, 0.5, 0.0, 1.0 };
    GLfloat diff[] = { 1.0, 1.0, 0.5, 1.0 };
    GLfloat spec[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat pos[] = { 0.0, 0.0, 0.0, 1.0 };

    glLightfv(GL_LIGHT1, GL_POSITION, pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec);
}

// ------------------------ Material ------------------------
void Material_Sun() {
    GLfloat amb[] = { 0.2, 0.0, 0.0, 1.0 };
    GLfloat diff[] = { 1.0, 0.5, 0.5, 1.0 };
    GLfloat spec[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat emis[] = { 0.3, 0.1, 0.1, 0.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emis);
}

void Material_Moon() {
    GLfloat amb[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat diff[] = { 0.5, 0.5, 0.1, 1.0 };
    GLfloat spec[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat shiny[] = { 100.0 };
    GLfloat emis[] = { 0.3, 0.3, 0.1, 0.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emis);
}

void Material_Earth() {
    GLfloat amb[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat diff[] = { 0.1, 0.1, 0.8, 1.0 };
    GLfloat spec[] = { 0.5, 0.5, 1.0, 1.0 };
    GLfloat shiny[] = { 80.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, shiny);
}

// ------------------------ Texture ------------------------
GLuint g_textureID[2];

void loadTexture() {
    AUX_RGBImageRec* pEarthImage = auxDIBImageLoad("C:\\Users\\eunse\\source\\repos\\ComputerGraphic\\Earth.bmp");
    AUX_RGBImageRec* pSunImage = auxDIBImageLoad("C:\\Users\\eunse\\source\\repos\\ComputerGraphic\\Sun.bmp");

    if (pEarthImage != NULL && pSunImage != NULL) {
        glGenTextures(2, &g_textureID[0]);

        // Earth
        glBindTexture(GL_TEXTURE_2D, g_textureID[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, pEarthImage->sizeX, pEarthImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pEarthImage->data);

        // Sun
        glBindTexture(GL_TEXTURE_2D, g_textureID[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, pSunImage->sizeX, pSunImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pSunImage->data);

        if (pEarthImage->data) free(pEarthImage->data);
        if (pSunImage->data) free(pSunImage->data);
        free(pEarthImage);
        free(pSunImage);
    }
}

// ------------------------ Display ------------------------
void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);

    // ----------- Sun -----------
    glPushMatrix();
    Material_Sun();
    Light_Sun();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_textureID[1]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    GLUquadric* quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    glColor3f(1.0, 0.3, 0.3); // 텍스처 색 보장
    gluSphere(quadric, 0.2, 30, 24);
    gluDeleteQuadric(quadric);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // ----------- Earth -----------
    glPushMatrix();
    glRotatef(Day, 0, 1, 0);
    glTranslatef(0.7, 0, 0);

    glDisable(GL_COLOR_MATERIAL);
    Material_Earth();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, g_textureID[0]);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    glColor3f(0.5, 0.6, 0.7);
    gluSphere(quadric, 0.1, 20, 16);
    gluDeleteQuadric(quadric);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);

    // ----------- Moon -----------
    glPushMatrix();
    glRotatef(Time, 0, 1, 0);
    glTranslatef(0.2, 0, 0);

    Material_Moon();
    Light_Moon();
    glutSolidSphere(0.05, 20, 16);

    glPopMatrix(); // Moon
    glPopMatrix(); // Earth

    glutSwapBuffers();
}

// ------------------------ Timer ------------------------
void MyTimer(int Value) {
    Day = (Day + 1) % 360;
    Time = (Time + 5) % 360;

    mat4 toOrigin = translate(mat4(1), -at);
    mat4 rot = rotate(mat4(1), radians(1.f), vec3(0, 0, 1));
    mat4 fromOrigin = translate(mat4(1), at);

    eye = vec3(fromOrigin * rot * toOrigin * vec4(eye, 1));
    up = vec3(rot * vec4(up, 0));

    glutPostRedisplay();
    glutTimerFunc(40, MyTimer, 1);
}

// ------------------------ Reshape ------------------------
void MyReshape(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(20.0, (float)w / h, 0.1, 20.0);
}

// ------------------------ Main ------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("234562_hw8");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 0, 1);

    loadTexture(); // 한 번만 호출

    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutTimerFunc(40, MyTimer, 1);

    glutMainLoop();
    return 0;
}


*/