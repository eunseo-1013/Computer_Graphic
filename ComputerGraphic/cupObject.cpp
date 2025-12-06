#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>


/*
using namespace std;



int windowWidth = 1280;
int windowHeight = 720;

// 마우스 회전 관련 전역 변수
float angleX = 0.0f;
float angleY = 0.0f;
int   prevX, prevY;
bool  isDragging = false;

// GLU Quadric (필요시 전역 사용 가능)
GLUquadric* quad = NULL;

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
    GLfloat matShininess[] = { 100.0f };             // 반짝임 강함

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

    // --------------------------
    // 파라미터들
    // --------------------------
    float outerBottom = 0.5f;
    float outerTop = 0.8f;
    float height = 2.0f;

    float thickness = 0.05f;          // 유리 두께
    float innerBottom = outerBottom - thickness;
    float innerTop = outerTop - thickness;

    int slices = 40;

    // --------------------------
    // 투명 유리 효과
    // --------------------------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    // --------------------------
    // 1) 컵 외벽
    // --------------------------
    gluCylinder(quad, outerBottom, outerTop, height, slices, slices);

    // 1-1) 컵 외부 바닥 (바깥쪽 면)
    gluDisk(quad, 0.0f, outerBottom, slices, 1);

    // --------------------------
    // 2) 컵 내부 벽 (두께 표현)
    //    안쪽을 향하도록 뒤집어서 그림
    // --------------------------
    glPushMatrix();
    glTranslatef(0, 0, height);
    glRotatef(180, 1, 0, 0); // 위에서 아래로 뒤집기
    gluCylinder(quad, innerTop, innerBottom, height-0.2, slices, slices);
    glPopMatrix();

    // --------------------------
    // 3) 윗 테두리(컵 입구) 링
    //    innerTop ~ outerTop 사이를 막아줌
    // --------------------------
    glPushMatrix();
    glTranslatef(0, 0, height); // 컵 윗부분 z = height 위치
    gluDisk(quad, innerTop, outerTop, slices, 1);
    glPopMatrix();

    // --------------------------
    // 4) 바닥 두께 링 (outerBottom ~ innerBottom)
    // --------------------------
    gluDisk(quad, innerBottom, outerBottom, slices, 1);

    // --------------------------
    // 5) 컵 안쪽 바닥 (사용자가 보는 내부 바닥)
    // --------------------------
    glPushMatrix();
    glTranslatef(0, 0, 0.01f); // Z-fighting 방지용 약간 올림
    gluDisk(quad, 0.0f, innerBottom, slices, 1);
    glPopMatrix();

    // --------------------------
    // 마무리
    // --------------------------
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    glPopMatrix();
    gluDeleteQuadric(quad);
}



// --------------------------
// 디스플레이 콜백 (실제 사용)
// --------------------------
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

/*  필요하면 예전 버전 디스플레이를 참고용으로 이렇게 보관해도 됨

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
// 마우스 콜백 (드래그로 회전)
// --------------------------
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