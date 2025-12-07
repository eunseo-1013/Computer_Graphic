#define _CRT_SECURE_NO_WARNINGS
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

/// 1 loadtexture 합치기  o
/// 2. main 에 페이지 번호 추가하기 
/// 3. 게이지  바 추가하기 o
/// 4. skybox 이용해서 모든 화면에 배경 추가 하기 
/// 5. 클릭하면 게이지 바 올라가기 
/// 6. 조작 통일하기
///6 -> 4  ->  5,2



//------------------------------
// 텍스처 전역 변수
//------------------------------
GLuint gLeafTex = 0; // 잎 텍스처 번호
GLuint gSoilTex = 0; // 흙 텍스처 번호
GLuint gStemTex = 0;


//------------------------------
// 조명 설정
//------------------------------
void SetupLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);          // 스케일 변형해도 노멀 자동 보정
    glEnable(GL_COLOR_MATERIAL);     // glColor로 재질 색 조절
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // 화면 전체 기본 밝기
    GLfloat globalAmbient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    // 태양 느낌의 따뜻한 흰빛
    GLfloat lightAmbient[] = { 0.4f, 0.4f, 0.35f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 0.98f, 0.90f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 0.95f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
}



//------------------------------
// 마우스 회전 관련
//------------------------------
float angleX = 0.0f;
float angleY = 0.0f;
int prevX, prevY;
bool isDragging = false;

// GLU Quadric
GLUquadric* quad = NULL;

//------------------------------
// 1. 화분 및 흙
//------------------------------

void FlowerPot(float size) {
    

    // ----- 0. 조명 상태 백업 -----
    GLfloat oldAmb[4], oldDiff[4], oldSpec[4];
    glGetLightfv(GL_LIGHT0, GL_AMBIENT, oldAmb);
    glGetLightfv(GL_LIGHT0, GL_DIFFUSE, oldDiff);
    glGetLightfv(GL_LIGHT0, GL_SPECULAR, oldSpec);

    GLfloat potAmb[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat potDiff[] = { 0.9f, 0.9f, 0.9f, 1.0f }; // 살짝 줄임
    GLfloat potSpec[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, potAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, potDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, potSpec);

    // ----- 2. 화분 재질 설정 (specular 0, 하이라이트 없음) -----
    glDisable(GL_COLOR_MATERIAL);    // glColor 영향 끄고 재질로만 색 지정

    GLfloat matAmb[] = { 0.30f, 0.18f, 0.10f, 1.0f }; // 어두운 갈색
    GLfloat matDiff[] = { 0.50f, 0.30f, 0.16f, 1.0f }; // 본 색
    GLfloat matSpec[] = { 0.0f, 0.0f, 0.0f, 1.0f };    // 반짝임 없음
    GLfloat matShine[] = { 1.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShine);
    GLdouble plane[] = { 0.0, 0.0, -1, 0.7 };
    glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, plane);
    glColor3f(0.7f, 0.4f, 0.2f); // 밝은 화분 갈색
    glutSolidCone(size, size + 1, 30, 30);
    glDisable(GL_CLIP_PLANE0);
    if (!quad)
        quad = gluNewQuadric();

    glPushMatrix();
   // glColor3f(0.6f, 0.3f, 0.2f);
    // SolidCone 높이 = size + 1
    float topZ = (size) * 0.10f; // ← 클립된 상단 실제 위치에 맞춰 보정

    glTranslatef(0, 0, -topZ);

    float rimH = 0.15f;
    float rimOuter = size * 1.0f;
    float rimInner = size * 0.01f;

    // Outer rim
    gluCylinder(quad, rimOuter, rimOuter, rimH, 30, 1);

    // Inner rim
    gluCylinder(quad, rimInner, rimInner, rimH, 30, 1);

    glPopMatrix();


     // 아래쪽 링

    glPushMatrix();
    //glColor3f(0.6f, 0.3f, 0.2f);
    // SolidCone 높이 = size + 1
    topZ = (size) * 0.67f; // ← 클립된 상단 실제 위치에 맞춰 보정

    glTranslatef(0, 0, topZ*2);

     rimH = 0.1f;
     rimOuter = size * 0.55f;
     rimInner = size * 0.01f;

    // Outer rim
    gluCylinder(quad, rimOuter, rimOuter, rimH, 30, 1);

    // Inner rim
    gluCylinder(quad, rimInner, rimInner, rimH, 30, 1);

    glPopMatrix();
    // ----- 4. 상태 원복 -----
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1.0f, 1.0f, 1.0f); // 다음 물체에 색 안 묻도록

    glLightfv(GL_LIGHT0, GL_AMBIENT, oldAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, oldDiff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, oldSpec);
}

void Soild() {
    // 텍스처 없으면 색만
    if (gSoilTex == 0) {
        GLdouble plane[] = { 0.0, 0.0, -1, 0.9 };
        glEnable(GL_CLIP_PLANE0);
        glClipPlane(GL_CLIP_PLANE0, plane);
        glColor3f(0.55f, 0.35f, 0.18f); // 조금 밝은 흙색
        glutSolidCone(0.4, 0.1, 30, 30);
        glDisable(GL_CLIP_PLANE0);
        return;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gSoilTex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    float radius = 0.40f;
    int   slices = 40;
    float z = 0.01f;

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, 0.0f, z);

    for (int i = 0; i <= slices; ++i) {
        float theta = (float)i / (float)slices * 2.0f * 3.14159265f;
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);

        float u = 0.5f + 0.5f * cosf(theta);
        float v = 0.5f + 0.5f * sinf(theta);

        glTexCoord2f(u, v);
        glVertex3f(x, y, z);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//------------------------------
// 2. 줄기 및 잎
//------------------------------
void Stem(float r, float h, float slice = 30) {
    if (!quad) {
        quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH);   // 조명용 노멀
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gStemTex);    // 줄기 텍스처
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // 텍스처가 원통 전체에 꽉 차게 / 반복되게
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // ★ 여기서 꼭 켜야 GLU가 텍스처 좌표를 만들어 줌
    gluQuadricTexture(quad, GL_TRUE);

    glColor3f(0.0f, 0.58f, 0.0f); // 줄기 기본 색
    gluQuadricDrawStyle(quad, GLU_FILL);
    /*
    // ─ 텍스처 더 자주/덜 자주 반복시키고 싶을 때: 텍스처 행렬 스케일 ─
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    // t방향을 3배 늘려서 세로 방향으로 무늬가 더 많이 반복되게 (취향대로 수정)
    glScalef(1.0f, 3.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);*/ 

    gluCylinder(quad, r, r-0.025, h, slice, 1);

    // 텍스처 행렬 복구
    glMatrixMode(GL_TEXTURE);
    //glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // 다른 도형에 영향 안 가게 정리
    gluQuadricTexture(quad, GL_FALSE);
    glDisable(GL_TEXTURE_2D);
}


// 토마토 꼭대기 작은 잎(삼각형) - 텍스처 사용
void TomatoLeaf() {
    glRotatef(90, -1, 0, 0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gLeafTex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor3f(0.0f, 0.32f, 0.0f);

    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.0f, 1.0f);

    glTexCoord2f(0.05f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.005f);

    glTexCoord2f(0.10f, 0.0f);
    glVertex3f(0.03f, 0.0f, 0.005f);

    glTexCoord2f(0.075f, 0.10f);
    glVertex3f(0.005f, 0.05f, 0.005f);

    glEnd();

    glDisable(GL_TEXTURE_2D);  
}

void TomatoCapLeaves(float r) {
    glTranslatef(0.0f, r + 0.01f, 0.0f);
    for (int i = 0; i < 3; ++i) {
        glPushMatrix();
        glRotatef(i * 120.0f, 0, 1, 0);
        TomatoLeaf();
        glPopMatrix();
    }
}

// 토마토 본체(구)
void Tomato(float r) {
    glPushMatrix();

    // 토마토 구는 텍스처 없이 재질/조명으로만
    glDisable(GL_TEXTURE_2D);

    glDisable(GL_COLOR_MATERIAL);
    GLfloat matAmbient[] = { 0.35f, 0.05f, 0.05f, 1.0f };  // 톤 유지
    GLfloat matDiffuse[] = { 0.85f, 0.18f, 0.18f, 1.0f };  // 붉은 채도 유지
    GLfloat matSpecular[] = { 0.5f, 0.6f, 0.5f, 1.0f };     // ★ 약한 하이라이트
    GLfloat matShininess[] = { 35.0f };

    // 살짝 스스로 빛나는 느낌
    GLfloat matEmission[] = { 0.2f, 0.03f, 0.03f, 1.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);

    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(r, 30, 30);

    // emission 원상복구
    GLfloat zeroEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, zeroEmission);

    // 꼭대기 잎은 텍스처 사용
    glEnable(GL_TEXTURE_2D);
    TomatoCapLeaves(r);

    glEnable(GL_COLOR_MATERIAL);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPopMatrix();
}

// 큰 잎(브랜치 잎) – 이미 잘 되어 있어서 텍스처 켰다가 끄기만 추가
void DrawTomatoLeaf(float len, float wid) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gLeafTex);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor3f(1.0f, 1.0f, 1.0f);

    float length = len;
    float width = wid;
    float z_bulge = 0.05;
    glNormal3f(0.0f, 0.0f, 1.0f);

    // 왼쪽
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0.05f, 0.0f);  glVertex3f(0, 0, 0);
    glTexCoord2f(0.025f, 0.0f);  glVertex3f(-width / 4, 0, 0);

    glTexCoord2f(0.05f, 0.04f); glVertex3f(0, length * 0.4f, z_bulge);
    glTexCoord2f(0.0f, 0.04f); glVertex3f(-width / 2, length * 0.4f, 0);

    glTexCoord2f(0.05f, 0.08f); glVertex3f(0, length * 0.8f, z_bulge / 2);
    glTexCoord2f(0.025f, 0.08f); glVertex3f(-width / 4, length * 0.8f, 0);

    glTexCoord2f(0.05f, 0.1f);  glVertex3f(0, length, 0);
    glTexCoord2f(0.05f, 0.1f);  glVertex3f(0, length, 0);
    glEnd();

    // 오른쪽
    glBegin(GL_QUAD_STRIP);
    glTexCoord2f(0.05f, 0.0f);  glVertex3f(0, 0, 0);
    glTexCoord2f(0.075f, 0.0f);  glVertex3f(width / 4, 0, 0);

    glTexCoord2f(0.05f, 0.04f); glVertex3f(0, length * 0.4f, z_bulge);
    glTexCoord2f(0.1f, 0.04f); glVertex3f(width / 2, length * 0.4f, 0);

    glTexCoord2f(0.05f, 0.08f); glVertex3f(0, length * 0.8f, z_bulge / 2);
    glTexCoord2f(0.075f, 0.08f); glVertex3f(width / 4, length * 0.8f, 0);

    glTexCoord2f(0.05f, 0.1f);  glVertex3f(0, length, 0);
    glTexCoord2f(0.05f, 0.1f);  glVertex3f(0, length, 0);
    glEnd();

    // 중앙 잎맥
    glColor3f(0.0f, 0.4f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, length, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);   // ★ 다음 물체에 텍스처 안 묻도록
}

// 토마토 + 가지 묶음
void TomatoDisplay(float stem_h, float angle_y, int count = 2) {
    glPushMatrix();

    glTranslatef(0.0f, stem_h, 0.0f);
    glRotatef(angle_y, 0.0f, 1.0f, 0.0f);

    float branch_start_offset = 0.01f;
    glTranslatef(0.0f, 0.2f, branch_start_offset);

    // 잎 묶음
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glTranslatef(0.02f, 0, -0.02f);

    glPushMatrix();
    DrawTomatoLeaf(0.13f, 0.1f);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-60.0f, 0.0f, 0.0f, 1.0f);
    DrawTomatoLeaf(0.15f, 0.13f);
    glPopMatrix();

    glPopMatrix();

    glRotatef(20, 1.0f, 0.0f, 0.0f);

    // 가지 줄기
    float branch_length = 0.18f;
    glPushMatrix();
    glColor3f(0.0f, 0.28f, 0.0f);

    if (!quad) quad = gluNewQuadric();
    gluCylinder(quad, 0.012, 0.005, branch_length, 10, 1);
    glPopMatrix();

    // 토마토 열매
    glPushMatrix();
    glTranslatef(0.0f, -0.1f, branch_length);
    glRotatef(-40, 1, 0, 0);
    glRotatef(30, 1.0f, 0.0f, 0.0f);

    glPushMatrix();
    Tomato(0.11f);
    glPopMatrix();

    if (count >= 2) {
        glPushMatrix();
        glTranslatef(0.08f, -0.05f, 0.08f);
        glRotatef(15, 0, 1, 0);
        Tomato(0.09f);
        glPopMatrix();
    }

    glPopMatrix();
    glPopMatrix();
}

//------------------------------
// 디스플레이
//------------------------------
void MyDisplay() {
   
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //DrawSkybox();
    //gluLookAt(0, 3, 3, 0, 0, 0, 0, 1, 0);
    gluLookAt(
        cam.eye.x, cam.eye.y, cam.eye.z,
        cam.at.x, cam.at.y, cam.at.z,
        cam.up.x, cam.up.y, cam.up.z
    );
 
    // 배경
    DrawSkybox();

    // 중심 (400, 300), 반지름 150, 두께 30
   

    GLfloat lightPos[] = { 1.5f, 3.0f, 2.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    // 조명은 항상 켜둠
    glEnable(GL_LIGHTING);

    // 화분 + 흙
    glPushMatrix();
    glTranslatef(0, -0.2f, 0);
    glRotatef(90, 1, 0, 0);
    FlowerPot(0.45f);
    glRotatef(-90, 1, 0, 0);
    glRotatef(90, -1, 0, 0);
    Soild();
    glPopMatrix();

    // 줄기
    glPushMatrix();
    glRotatef(90, -1, 0, 0);
    glTranslatef(0.0f, 0, -0.5f);
    Stem(0.03f, 1.5f, 30);
    glPopMatrix();

    // 맨 아래 큰 잎 한 장
    float initial_height = 0.1f;
    float height_step = 0.3f;
    float angle_step = 70.0f;
    int   total_clusters = 4;
    float angle_step2 = 80.0f;

    glPushMatrix();
    glRotatef(angle_step2, 0, 1, 0);
    glRotatef(90, 1, 0, 0);
    DrawTomatoLeaf(0.08f, 0.04f);
    glPopMatrix();

    // 토마토 묶음들
    for (int i = 0; i < total_clusters; i++) {
        float current_height = initial_height + (i * height_step);
        float current_angle = i * angle_step;
        int count = (i < 2) ? 2 : 1;
        if (i == 3) {
            count = 2;
            current_height = initial_height + height_step;
        }
        TomatoDisplay(current_height, current_angle, count);
    }

   

    // ------------------------------
    // 2) 2D HUD 모드로 변경
    // ------------------------------
    // 식물 위 gauge 높이
    float gaugeHeight = 1.1f;

    // 게이지는 조명 영향받으면 3D처럼 보이기 때문에 OFF
    glDisable(GL_LIGHTING);

    // 깊이 테스트만 끄기!
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();

    // 식물의 위 위치
    glTranslatef(-0.31f, gaugeHeight, 0.0f);

    // 카메라를 바라보도록 (지글거림 해결 핵심)
    //billboardToCamera();

    // 화면 크기 조절
    glScalef(0.4f, 0.4f, 0.4f);

    drawCircularGauge(0.8f, 0.5f, 0.6f, 0.2f, g_value);

    glPopMatrix();

    // 원래 상태 복구
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);


    glutSwapBuffers();
}




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

    glutDisplayFunc(MyDisplay);
    
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


