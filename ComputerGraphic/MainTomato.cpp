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
#include "water_kettle.h"
#include "WaterParticles.h"
#include "eye.h"
#include "main.h"

/// 1 loadtexture 합치기  o
/// 2. main 에 페이지 번호 추가하기 
/// 3. 게이지 바 추가하기 o
/// 4. skybox 이용해서 모든 화면에 배경 추가 하기  <-  ??? 안돼
/// 5. 클릭하면 게이지 바 올라가기 o
/// 6. 조작 통일하기 o
/// 7. 물통 불러오기  o
/// 4 ? -> 5,2

// 마우스 회전 관련
int num = pov-3;


// ------------------------------
// 디스플레이
// ------------------------------
void TomatoSceneDisplay()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(cam.fov, (float)windowWidth / (float)windowHeight, 0.1f, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };    // 테이블 색상 맞추려고
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    // 카메라
    gluLookAt(
        cam.eye.x, cam.eye.y, cam.eye.z,
        cam.at.x, cam.at.y, cam.at.z,
        cam.up.x, cam.up.y, cam.up.z
    );


    // 1) 배경 (원하면 사용)
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    DrawSkybox();
    glPopMatrix();
    

    // 2) 본 씬(토마토 + 물통)
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGHT0);

    glPushMatrix();
    glTranslatef(0.0f, 9.f, 0.0f);
    glPushMatrix(); // 0,9,0으로 이동 한것!

    // 조명 위치
    //GLfloat lightPos[] = { 1.5f, 3.0f, 2.0f, 1.0f };
    

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

    glPushMatrix();
    glTranslatef(0.0f, 0.51f, -0.3f); // 화분 옮기기 ( 위로 )
    glScalef(0.5f, 0.5f, 0.5f);

    // --- 화분 + 흙 ---
    glPushMatrix();
    glTranslatef(0.0f, -0.2f, 0.0f);
    glRotatef(90.0f, 1, 0, 0);
    FlowerPot(0.45f);
    glRotatef(-90.0f, 1, 0, 0);
    glRotatef(90.0f, -1, 0, 0);
    Soild();
    glPopMatrix();

    // --- 줄기 ---
    glPushMatrix();
    glRotatef(90.0f, -1, 0, 0);
    glTranslatef(0.0f, 0.0f, -0.5f);
    Stem(0.03f, 1.5f, 30);
    glPopMatrix();

    // --- 맨 아래 큰 잎 한 장 ---
    float initial_height = 0.1f;
    float height_step = 0.3f;
    float angle_step = 70.0f;
    int   total_clusters = 4;
    float angle_step2 = 80.0f;

    glPushMatrix();
    glRotatef(angle_step2, 0, 1, 0);
    glRotatef(90.0f, 1, 0, 0);
    DrawTomatoLeaf(0.08f, 0.04f);
    glPopMatrix();

    // --- 토마토 묶음들 ---
    for (int i = 0; i < total_clusters; ++i) {
        float current_height = initial_height + (i * height_step);
        float current_angle = i * angle_step;
        int count = (i < 2) ? 2 : 1;
        if (i == 3) {
            count = 2;
            current_height = initial_height + height_step;
        }
        TomatoDisplay(current_height, current_angle, count);  // Tomato.h 쪽 함수
    }

    glPopMatrix();
   

    static float t = 0.0f;
    t += 0.01f;
    waterTime = t;

    // --- 물주전자 (토마토와 같은 월드 공간) --- 0,9,0 이동 한곳!
    UpdateWaterKettle();
    vec3 pos = gKettleBasePos + glm::vec3(0.0f, kettleLift, 0.0f);
    //DrawWaterKettle(0.8f, 0.8f + kettleLift, -0.2f, waterTime, kettleAngle);

    DrawWaterKettle(pos.x-0.05f, pos.y + 0.351f, pos.z, waterTime, kettleAngle);
    DrawCrosshair();
    UpdateWaterParticles(kettleAngle, isPouring);
    DrawWaterParticles(pos.x-0.05f, pos.y-0.02f+0.351f, pos.z, kettleAngle);

   
    glPopMatrix(); //0,9,0 pop -> 0,0,0 좌표로 이동
    glPopMatrix(); // 씬 전체 매트릭스


  
    
    // 3) HUD 게이지 (조명/깊이 끄고)
    float gaugeHeight = 10.1f;

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glPushMatrix();
    glTranslatef(-0.31f, gaugeHeight, -0.3f);
    glScalef(0.4f, 0.4f, 0.4f);
    drawCircularGauge(0.8f, 0.3f, 0.3f, 0.1f, g_value);
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    if (pov >= 3) {
        DrawNoiseOverlay(0.7f, num);
    }
   
   
    glutSwapBuffers();
    

}

// ------------------------------
// 리쉐이프
// ------------------------------
void MyReshape(int w, int h)
{
    if (h == 0) h = 1;

    windowWidth = w;
    windowHeight = h;

    glViewport(0, 0, w, h);
}

// ------------------------------
// 텍스처 초기화 (토마토용)
// ------------------------------
bool InitTextures()
{

    gNoiseTex0 = gLoadTexture("texture/Particle.bmp");
    gNoiseTex1 = gLoadTexture("texture/noise.bmp");
    gNoiseTex2 = gLoadTexture("texture/noise2.bmp");
    gLeafTex = gLoadTexture("texture/24leaf_texture.bmp");
    gSoilTex = gLoadTexture("texture/soild_texture.bmp");
    gStemTex = gLoadTexture("texture/stem_texture.bmp");

    if (!gLeafTex || !gSoilTex || !gStemTex ) {
        printf("텍스처 로드 실패\n");
        return false;
    }
    return true;
}

// ------------------------------
// main
// ------------------------------
int TomatoMain(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);


    glutCreateWindow("Tomato");
  
    glutFullScreen();
    glewInit();
    glEnable(GL_TEXTURE_2D);

    // ★ 카메라 초기 세팅 (원하면 값 조절 가능)
    cam.eye = vec3(0.0f, 10.0f, 3.0f);
    cam.at = vec3(0.0f, 10.0f, 0.0f);
    cam.up = vec3(0.0f, 1.0f, 0.0f);
    cam.UpdateCamera();

    // 스카이박스 텍스처 (원하면 사용)
    InitSkybox();

    glShadeModel(GL_SMOOTH);
    SetupLighting();
    glEnable(GL_DEPTH_TEST);

    if (!InitTextures()) {
        return -1;
    }

    // 물주전자 텍스처 초기화
    InitWaterKettleTextures();
    InitWaterParticles();

    glClearColor(1, 1, 1, 1);

    if (!quad) quad = gluNewQuadric();

    // 콜백 등록
    glutDisplayFunc(TomatoSceneDisplay);
    glutReshapeFunc(MyReshape);

    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(mouseMove);
    glutSetCursor(GLUT_CURSOR_NONE);

    // 카메라 움직임 타이머
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
