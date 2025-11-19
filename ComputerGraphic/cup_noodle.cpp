#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "cup_noodle.h"
#include <iostream>
#include <vector>

using namespace std;

GLuint noodleTextureID;
GLuint cupTextureID;
GLuint greenTextureID;
GLuint eggTextureID;

GLuint loadTexture(const char* filename) {
    GLuint textureID;
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (!data) {
        cerr << "Failed to load texture: " << filename << endl;
        return 0;
    }

    GLenum format;
    if (nrChannels == 1) format = GL_RED;
    else if (nrChannels == 3) format = GL_RGB;
    else if (nrChannels == 4) format = GL_RGBA;
    else {
        cerr << "Unknown texture format." << endl;
        stbi_image_free(data);
        return 0;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
    return textureID;
}

void drawTexturedFlake() {
    // ¾Õ¸é
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();

    // µÞ¸é
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glEnd();

    // À­¸é
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glEnd();

    // ¾Æ·§¸é
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();

    // ¿À¸¥ÂÊ ¸é
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glEnd();

    // ¿ÞÂÊ ¸é
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();
}

void drawEggFlake() {
    GLUquadric* egg = gluNewQuadric();
    gluQuadricTexture(egg, GL_TRUE);
    gluCylinder(egg, 1.0f, 1.0f, 0.5f, 32, 32);
    gluDisk(egg, 0.0f, 1.0f, 32, 1);
    glTranslatef(0.0f, 0.0f, 0.5f);
    gluDisk(egg, 0.0f, 1.0f, 32, 1);
    gluDeleteQuadric(egg);
}

void InitCupNoodleTextures() {
    noodleTextureID = loadTexture("noodle.bmp");
    if (noodleTextureID == 0) cerr << "Failed to load noodleTexture." << endl;

    cupTextureID = loadTexture("styrofoam.bmp");
    if (cupTextureID == 0) cerr << "Failed to load cupTexture." << endl;

    greenTextureID = loadTexture("green.png");
    if (greenTextureID == 0) cerr << "Failed to load greenTexture." << endl;

    eggTextureID = loadTexture("egg.png");
    if (eggTextureID == 0) cerr << "Failed to load eggTexture." << endl;
}

void DrawCupNoodleScene() {
    // 1. Å×ÀÌºí
    glPushMatrix();
    glColor3f(0.5f, 0.35f, 0.05f);
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(3.0f, 0.1f, 1.5f);
    glutSolidCube(1.0);
    glPopMatrix();

    // 2. ÄÅ
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cupTextureID);

    glRotatef(-90.f, 1, 0, 0);
    glTranslatef(0.0f, 0.25f, 0.051f);
    GLUquadric* cup = gluNewQuadric();

    gluQuadricTexture(cup, GL_TRUE);
    gluCylinder(cup, 0.3f, 0.35f, 0.5f, 32, 32);

    // ÄÅ ¹Ù´Ú
    glColor3f(0.2f, 0.2f, 0.2f);
    gluDisk(cup, 0.0f, 0.3f, 32, 1);

    // ÄÅ ¶Ñ²± ºÙÀ» °ø°£
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, 0.0f, 0.5f);
    gluCylinder(cup, 0.35f, 0.35f, 0.01f, 32, 32);
    gluCylinder(cup, 0.37f, 0.37f, 0.01f, 32, 32);

    gluDisk(cup, 0.35f, 0.37f, 32, 1);
    glTranslatef(0.0f, 0.0f, 0.01f);
    gluDisk(cup, 0.35f, 0.37f, 32, 1);

    // ±âÁØ ¼±
    glColor3f(0.7f, 0.7f, 0.7f);
    glTranslatef(0.0f, 0.0f, -0.15f);

    gluCylinder(cup, 0.32f, 0.32f, 0.01f, 32, 32);
    gluDisk(cup, 0.32f, 0.325f, 32, 1);
    glTranslatef(0.0f, 0.0f, 0.01f);
    gluDisk(cup, 0.32f, 0.325f, 32, 1);

    gluDeleteQuadric(cup);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // 3. ¸é
    GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
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

    gluCylinder(noodle, 0.28f, 0.28f, 0.1f, 32, 32);
    gluDisk(noodle, 0.0f, 0.28f, 32, 1);
    glTranslatef(0.0f, 0.0f, 0.1f);
    gluDisk(noodle, 0.0f, 0.28f, 32, 1);
    gluDeleteQuadric(noodle);

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // 4. ½ºÇÁ
    glPushMatrix();
    glRotatef(-90.f, 1, 0, 0);
    glTranslatef(0.0f, 0.25f, 0.401f);

    // ÆÄ
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

    // °è¶õ
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

    // °íÃß
    glDisable(GL_TEXTURE_2D);
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

    glPopMatrix();
}