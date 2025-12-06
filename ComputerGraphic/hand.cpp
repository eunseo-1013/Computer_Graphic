#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>

/*
using namespace std;

int windowWidth = 1280;
int windowHeight = 720;

GLUquadric* quad = NULL;

// 마우스 회전 관련 전역 변수
float angleX = 0.0f;
float angleY = 0.0f;
int   prevX, prevY;
bool  isDragging = false;

// 컵 애니메이션 상태
enum CupState {
    CUP_STAND = 0,   // 서 있음
    CUP_FALLING,     // 넘어지는 중
    CUP_ROLLING      // 옆으로 굴러가는 중
};

CupState cupState = CUP_STAND;

// 컵 밑면 반지름 (cup_object에서 사용하는 값과 동일하게 유지)
const float CUP_RADIUS = 0.5f;

// 컵 넘어지는/굴러가는 애니메이션 관련
float fallAngle = 0.0f;   // 0 = 서 있음, -90 = 완전히 쓰러짐
float rollOffset = 0.0f;   // 책상 위에서 x 방향 이동량
float rollSpin = 0.0f;   // 컵이 바닥에서 구르면서 도는 각도

void AnimationTimer(int value);  // 타이머 콜백 프로토타입

// --------------------------
// 책상 그리기
// --------------------------
void drawTable() {
    // 책상은 단색으로, 간단히 조명 끄고 그려도 됨
    glDisable(GL_LIGHTING);

    glColor3f(0.7f, 0.5f, 0.3f);  // 나무색 느낌

    glBegin(GL_QUADS);
    glVertex3f(-5.0f, -1.0f, -5.0f);
    glVertex3f(5.0f, -1.0f, -5.0f);
    glVertex3f(5.0f, -1.0f, 5.0f);
    glVertex3f(-5.0f, -1.0f, 5.0f);
    glEnd();

    glEnable(GL_LIGHTING);
}

// --------------------------
// 조명 및 유리 재질 초기화
// --------------------------
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
    GLfloat matDiffuse[] = { 0.7f, 0.9f, 1.0f, 0.6f }; // 살짝 푸른빛
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 0.3f };
    GLfloat matShininess[] = { 100.0f };                 // 반짝임 강함

    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
}

// --------------------------
// 컵 오브젝트
// --------------------------
void cup_object() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    glPushMatrix();
    // GLU 원기둥은 z축 방향이 기본이므로,
    // -90도 회전해서 y축 방향(위로) 서 있게 만든다.
    glRotatef(-90, 1, 0, 0); // 컵을 위로 세우기

    float outerBottom = CUP_RADIUS;  // 0.5f
    float outerTop = 0.8f;
    float height = 2.0f;

    float thickness = 0.05f;          // 유리 두께
    float innerBottom = outerBottom - thickness;
    float innerTop = outerTop - thickness;

    int slices = 40;

    // 투명 유리 효과
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    // 1) 컵 외벽
    gluCylinder(quad, outerBottom, outerTop, height, slices, slices);

    // 1-1) 컵 외부 바닥 (바깥쪽 면)
    gluDisk(quad, 0.0f, outerBottom, slices, 1);

    // 2) 컵 내부 벽
    glPushMatrix();
    glTranslatef(0, 0, height);
    glRotatef(180, 1, 0, 0); // 위에서 아래로 뒤집기
    gluCylinder(quad, innerTop, innerBottom, height - 0.2f, slices, slices);
    glPopMatrix();

    // 3) 윗 테두리 링
    glPushMatrix();
    glTranslatef(0, 0, height);
    gluDisk(quad, innerTop, outerTop, slices, 1);
    glPopMatrix();

    // 4) 바닥 두께 링
    gluDisk(quad, innerBottom, outerBottom, slices, 1);

    // 5) 컵 안쪽 바닥
    glPushMatrix();
    glTranslatef(0, 0, 0.01f); // Z-fighting 방지
    gluDisk(quad, 0.0f, innerBottom, slices, 1);
    glPopMatrix();

    // 마무리
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glPopMatrix();
    gluDeleteQuadric(quad);
}

// --------------------------
// 애니메이션 타이머
// --------------------------
void AnimationTimer(int value) {
    const float fallSpeed = 3.0f;   // 넘어질 때 각속도 (deg/frame)
    const float rollSpeed = 0.02f;  // 옆으로 이동 속도 (조금 느리게)
    const float maxRoll = 1.0f;   // 최대 굴러가는 거리 (덜 굴러가게)

    if (cupState == CUP_FALLING) {
        // x축 기준으로 앞으로 넘어짐 (0 → -90도)
        fallAngle -= fallSpeed;

        // 책상에 '딱' 부딪힌 순간
        if (fallAngle <= -90.0f) {
            fallAngle = -90.0f;
            cupState = CUP_ROLLING;   // 이제부터 굴러감
        }
    }
    else if (cupState == CUP_ROLLING) {
        // 책상 위를 x 방향으로 굴러감
        rollOffset += rollSpeed;   // 오른쪽으로 이동

        if (rollOffset > maxRoll) {
            rollOffset = maxRoll;
        }

        // 이동한 거리만큼 회전 (진짜 굴러가는 각도)
        float dist = rollOffset;   // 현재까지 이동 거리
        rollSpin = -(dist / CUP_RADIUS) * (180.0f / 3.141592f);  // 도(degree)로 변환
    }

    glutPostRedisplay();

    // 아직 움직이는 상태면 타이머 다시 등록
    if (cupState == CUP_FALLING ||
        (cupState == CUP_ROLLING && rollOffset < maxRoll)) {
        glutTimerFunc(16, AnimationTimer, 0); // 약 60fps
    }
}

// --------------------------
// 디스플레이 콜백
// --------------------------
void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5,   // eye
        0, 0, 0,   // center
        0, 1, 0);  // up

    // 마우스 회전 (카메라 회전)
    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    // 1) 책상 먼저 그리기
    drawTable();

    // 2) 컵 그리기
    glPushMatrix();

    // 기본적으로 컵 밑이 y = -1의 책상 위에 닿도록 위치
    // -> 책상을 안 뚫게 하기 위해, 떨어질 때는 살짝 위로 올려서 보정
    if (cupState == CUP_FALLING) {
        // fallAngle 은 0 ~ -90 (deg)
        float rad = fallAngle * 3.141592f / 180.0f;
        float lift = CUP_RADIUS * (1.0f - cosf(rad));   // 대충 위로 올리는 양

        glTranslatef(0.0f, -1.0f + lift, 0.0f);
        glRotatef(fallAngle, 1.0f, 0.0f, 0.0f);
    }
    else if (cupState == CUP_ROLLING) {
        // 굴러가는 중: 책상 위에서 굴러가면서도 바닥을 뚫지 않도록
        // 중심이 테이블 위 CUP_RADIUS 만큼 떠 있게 놓고,
        // 옆으로 이동 + 회전
        glTranslatef(rollOffset, -1.0f + CUP_RADIUS, 0.0f);

        // x축으로 완전히 쓰러뜨린 상태 유지
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        // z축 기준으로 회전 → 바퀴처럼 굴러가는 느낌
        glRotatef(rollSpin, 0.0f, 0.0f, 1.0f);
    }
    else {
        // 서 있는 상태: 그냥 바닥에 딱 붙여서 세워놓기
        glTranslatef(0.0f, -1.0f, 0.0f);
    }

    cup_object();
    glPopMatrix();

    glutSwapBuffers();
}

// --------------------------
// 리쉐이프 콜백
// --------------------------
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

// --------------------------
// 마우스 콜백 (드래그 + 클릭으로 애니메이션 시작)
// --------------------------
void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            prevX = x;
            prevY = y;

            // 컵이 서 있는 상태에서만 새 애니메이션 시작
            if (cupState == CUP_STAND) {
                fallAngle = 0.0f;
                rollOffset = 0.0f;
                rollSpin = 0.0f;

                cupState = CUP_FALLING;
                glutTimerFunc(16, AnimationTimer, 0);
            }
        }
        else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
}

// --------------------------
// 마우스 드래그 시 카메라 회전
// --------------------------
void Motion(int x, int y) {
    if (isDragging) {
        int dx = x - prevX;
        int dy = y - prevY;

        angleY += dx * 0.3f;
        angleX += dy * 0.3f;

        if (angleX > 360.0f)  angleX -= 360.0f;
        if (angleX < -360.0f) angleX += 360.0f;
        if (angleY > 360.0f)  angleY -= 360.0f;
        if (angleY < -360.0f) angleY += 360.0f;

        prevX = x;
        prevY = y;

        glutPostRedisplay();
    }
}

// --------------------------
// 키보드 콜백
// --------------------------
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC
        glutLeaveMainLoop();
    }
}

// --------------------------
// 메인
// --------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Transparent Glass Cup: Falling & Rolling");

    /*
    // GLEW 초기화 (필수 환경이면)
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        cerr << "Error: " << glewGetErrorString(err) << endl;
        return -1;
    }
    */
/*

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