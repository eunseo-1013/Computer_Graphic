#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Camera.h"
#include "stb_image.h"

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
GLuint noodleTextureID;
GLuint cupTextureID;
GLuint greenTextureID;
GLuint eggTextureID;

void init();
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void mouseMove(int x, int y);
void moveCamera(int value);
void drawScene();

GLuint loadTexture(const char* filename) {
    GLuint textureID;
    int width, height, nrChannels;

    // 이미지를 로드 (stb_image가 y축을 뒤집기 때문에 수직으로 뒤집음)
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (!data) {
        cerr << "Failed to load texture: " << filename << endl;
        return 0;
    }

    // 채널 수에 따라 GL 포맷 결정
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;  // .jpg 또는 .bmp
    else if (nrChannels == 4)
        format = GL_RGBA; // .png
    else {
        cerr << "Unknown texture format." << endl;
        stbi_image_free(data);
        return 0;
    }

    // OpenGL 텍스처 생성
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // Mipmap 생성 
    glGenerateMipmap(GL_TEXTURE_2D);

    // 텍스처 필터링 설정
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 원본 이미지 데이터 메모리 해제
    stbi_image_free(data);

    return textureID;
}

void drawTexturedFlake() {
    // 앞면
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f); // 법선 벡터
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();

    // 뒷면
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();

    // 윗면
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glEnd();

    // 아랫면
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();

    // 오른쪽 면
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glEnd();

    // 왼쪽 면
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();
}

void drawEggFlake()
{
    GLUquadric* egg = gluNewQuadric();

    gluQuadricTexture(egg, GL_TRUE);

    gluCylinder(egg, 1.0f, 1.0f, 0.5f, 32, 32);  // 납작한 원기둥
    gluDisk(egg, 0.0f, 1.0f, 32, 1);               // 아랫면 닫기
    glTranslatef(0.0f, 0.0f, 0.5f);
    gluDisk(egg, 0.0f, 1.0f, 32, 1);               // 윗면 닫기
    gluDeleteQuadric(egg);
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
    glutPassiveMotionFunc(mouseMove);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutTimerFunc(16, moveCamera, 0);

    glutMainLoop();
    return 0;
}

void init() {
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    /*GLfloat light_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    GLfloat light1_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);*/

    GLfloat light_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat back_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // 뒷면 재질
    GLfloat back_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_BACK, GL_AMBIENT, back_ambient);
    glMaterialfv(GL_BACK, GL_DIFFUSE, back_diffuse);

    glEnable(GL_NORMALIZE);

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    cam.InitCamera(vec3(0.0f, 1.0f, 3.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f));

    for (int i = 0; i < 256; i++) keys[i] = false;

    noodleTextureID = loadTexture("noodle.bmp");
    if (noodleTextureID == 0) {
        cerr << "Failed to load noodelTexture." << endl;
    }

    cupTextureID = loadTexture("styrofoam.bmp");
    if (cupTextureID == 0) {
        cerr << "Failed to load cupTexture." << endl;
    }

    greenTextureID = loadTexture("green.png");
    if (greenTextureID == 0) {
        cerr << "Failed to load greenTexture." << endl;
    }

    eggTextureID = loadTexture("egg.png");
    if (eggTextureID == 0) {
        cerr << "Failed to load eggTexture." << endl;
    }
}

void drawScene() {
    // 1. 테이블
    glPushMatrix();
    glColor3f(0.5f, 0.35f, 0.05f);
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(3.0f, 0.1f, 1.5f);
    glutSolidCube(1.0);
    glPopMatrix();

    // 2. 컵
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cupTextureID);

    glRotatef(-90.f, 1, 0, 0);
    glTranslatef(0.0f, 0.25f, 0.051f);  // 테이블보다 0.001만큼 높여서 z-fighting 현상 해결
    GLUquadric* cup = gluNewQuadric();

    gluQuadricTexture(cup, GL_TRUE);

    gluCylinder(cup, 0.3f, 0.35f, 0.5f, 32, 32);

    // 컵 바닥
    glColor3f(0.2f, 0.2f, 0.2f);
    gluDisk(cup, 0.0f, 0.3f, 32, 1);

    // 컵 뚜껑 붙을 공간
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, 0.5f);
    gluCylinder(cup, 0.35f, 0.35f, 0.01f, 32, 32);
    gluCylinder(cup, 0.37f, 0.37f, 0.01f, 32, 32);

    gluDisk(cup, 0.35f, 0.37f, 32, 1);
    glTranslatef(0.0f, 0.0f, 0.01f);
    gluDisk(cup, 0.35f, 0.37f, 32, 1);

    // 기준 선
    glColor3f(0.7f, 0.7f, 0.7f);
    glTranslatef(0.0f, 0.0f, -0.15f);
    
    gluCylinder(cup, 0.32f, 0.32f, 0.01f, 32, 32);
    gluDisk(cup, 0.32f, 0.325f, 32, 1);
    glTranslatef(0.0f, 0.0f, 0.01f);
    gluDisk(cup, 0.32f, 0.325f, 32, 1);

    gluDeleteQuadric(cup);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // 3. 면
    GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // 광택 제거
    GLfloat mat_shininess[] = { 0.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, noodleTextureID);

    glRotatef(-90.f, 1, 0, 0);  
    glTranslatef(0.0f, 0.25f, 0.3f);
    GLUquadric* noodle = gluNewQuadric(); 

    gluQuadricTexture(noodle, GL_TRUE);

    gluCylinder(noodle, 0.28f, 0.28f, 0.1f, 32, 32);  // 납작한 원기둥
    gluDisk(noodle, 0.0f, 0.28f, 32, 1);               // 아랫면 닫기
    glTranslatef(0.0f, 0.0f, 0.1f);
    gluDisk(noodle, 0.0f, 0.28f, 32, 1);               // 윗면 닫기
    gluDeleteQuadric(noodle);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // 4. 스프
    glPushMatrix();
    glRotatef(-90.f, 1, 0, 0);
    glTranslatef(0.0f, 0.25f, 0.401f);
    // 파
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, greenTextureID);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glRotatef(-25.f, 0, 0, 1);
    glTranslatef(0.15f, -0.05f, 0.0f); 
    glScalef(0.05f, 0.02f, 0.005f); 
    drawTexturedFlake();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.1f, 0.1f, 0.001f);
    glScalef(0.04f, 0.03f, 0.005f);
    drawTexturedFlake();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.05f, 0.18f, 0.0f);
    glScalef(0.04f, 0.05f, 0.005f);
    drawTexturedFlake();
    glPopMatrix();

    // 계란
    glColor3f(1.0f, 1.0f, 0.6f); 
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, eggTextureID);

    glTranslatef(0.1f, 0.15f, 0.0f);
    glScalef(0.015f, 0.015f, 0.01f);
    drawEggFlake();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.15f, 0.05f, -0.002f);
    glScalef(0.017f, 0.017f, 0.01f);
    drawEggFlake();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, -0.03f, -0.0015f);
    glScalef(0.015f, 0.015f, 0.01f);
    drawEggFlake();
    glPopMatrix();

    // 고추
    glColor3f(0.8f, 0.2f, 0.2f); 
    glPushMatrix();
    glTranslatef(-0.1f, -0.1f, 0.001f);
    glScalef(0.02f, 0.03f, 0.005f);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.18f, 0.05f, 0.001f);
    glScalef(0.03f, 0.01f, 0.005f);
    glutSolidCube(1.0);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 헤드조명
    /*GLfloat headlamp_pos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, headlamp_pos);*/

    gluLookAt(cam.eye.x, cam.eye.y, cam.eye.z,
        cam.at.x, cam.at.y, cam.at.z,
        cam.up.x, cam.up.y, cam.up.z);

    // 천장조명
   /* GLfloat world_light_pos[] = { 1.0f, 3.0f, 3.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, world_light_pos);*/

    drawScene();
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

void keyboardUp(unsigned char key, int x, int y) {
    keys[key] = false;
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
