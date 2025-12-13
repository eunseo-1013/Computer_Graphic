#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "cup_noodle.h"
#include "eye.h"
#include <iostream>
#include <vector>

using namespace std;

GLuint noodleTextureID;
GLuint cupTextureID;
GLuint greenTextureID;
GLuint eggTextureID;
GLuint activewaterTextureID;



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



void waterCupNoodle(float gauge)
{
    if (gauge <= 0.0f) return;

    if (gauge < 0.0f) gauge = 0.0f;
    if (gauge > 1.0f) gauge = 1.0f; 
     
    const float cupHeight = 0.5f;
    const float bottomRadius = 0.3f;
    const float topRadius = 0.35f;

    float waterHeight = cupHeight * gauge;
    float waterBottomRadius = bottomRadius;
    float waterTopRadius =
        bottomRadius + (topRadius - bottomRadius) * (waterHeight / cupHeight);

    glPushMatrix();
    glRotatef(-90.f, 1, 0, 0);
    glTranslatef(0.0f, 0.25f, 0.051f);

    float epsilon = 0.001f;
    glTranslatef(0.0f, 0.0f, epsilon);

    // 조명 제거 + 알파 블렌딩
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // -------텍스처 활성화 --------
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, activewaterTextureID);  // 이미 로드된 텍스처 ID 사용
    //--------------------------------

    // 물 색 + 투명도 ( 반투명 빨강)
    glColor4f(1.0f, 0.35f, 0.1f, 0.5f);
    //  텍스처 사용 시 보통 색을 흰색(1,1,1)으로 둬야 텍스처 색이 그대로 나옴

    GLUquadric* water = gluNewQuadric();
    gluQuadricNormals(water, GLU_SMOOTH);
    gluQuadricTexture(water, GL_TRUE);   // 텍스처 ON

    // 물 벽면 (원뿔대)
    gluCylinder(water, waterBottomRadius, waterTopRadius, waterHeight, 32, 1);

    // 수면 텍스처 (위 디스크)
    glTranslatef(0.0f, 0.0f, waterHeight);

    // 수면 디스크도 텍스처 적용
    gluDisk(water, 0.0f, waterTopRadius, 32, 1);

    gluDeleteQuadric(water);

    // ------- 텍스처 OFF --------
    //glDisable(GL_TEXTURE_2D);

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}




void drawTexturedFlake() {
    // 앞면
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
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
    noodleTextureID = loadTexture("texture/noodle.bmp");
    if (noodleTextureID == 0) cerr << "Failed to load noodleTexture." << endl;

    cupTextureID = loadTexture("texture/styrofoam.bmp");
    if (cupTextureID == 0) cerr << "Failed to load cupTexture." << endl;

    greenTextureID = loadTexture("texture/green.png");
    if (greenTextureID == 0) cerr << "Failed to load greenTexture." << endl;

    eggTextureID = loadTexture("texture/egg.png");
    if (eggTextureID == 0) cerr << "Failed to load eggTexture." << endl;
    activewaterTextureID = loadTexture("texture/water3.bmp");
    if (activewaterTextureID == 0) cerr << "Failed to load waterTextureID." << endl;

  
}

void DrawCupNoodleScene() {
    glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);
    
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0.0f, 8.8f, 0.0f);

    // 1. 테이블
   // --------------------
// 테이블 상판
// --------------------
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glColor3f(0.5f, 0.35f, 0.05f);   // 나무색
    glScalef(3.0f, 0.1f, 1.5f);      // 상판 크기
    glutSolidCube(1.0);
    glPopMatrix();

    // --------------------
    // 테이블 다리
    // --------------------
    glColor3f(0.45f, 0.30f, 0.08f);  // 다리 조금 더 진하게

    float legHeight = 1.2f;
    float legThickness = 0.15f;

    // 다리 위치 (상판 기준 네 귀퉁이)
    float xOffset = 1.4f;
    float zOffset = 0.65f;

    for (int x = -1; x <= 1; x += 2)
    {
        for (int z = -1; z <= 1; z += 2)
        {
            glPushMatrix();
            glTranslatef(
                x * xOffset,
                -legHeight * 0.5f - 0.05f,   // 상판 아래로
                z * zOffset
            );
            glScalef(legThickness, legHeight, legThickness);
            glutSolidCube(1.0);
            glPopMatrix();
        }
    }

    glEnable(GL_TEXTURE_2D);

    // 2. 컵
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cupTextureID);

    glRotatef(-90.f, 1, 0, 0);
    glTranslatef(0.0f, 0.25f, 0.051f);
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
    
    glPopMatrix();

    // 4. 스프
    glPushMatrix();
    glRotatef(-90.f, 1, 0, 0);
    glTranslatef(0.0f, 0.0f, 8.8f);
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
    //glPopMatrix();
    glPopAttrib();
}