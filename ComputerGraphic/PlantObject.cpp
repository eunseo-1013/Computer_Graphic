#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glut.h>

#include <GL/glut.h>


/*


float angleX = 0.0f;  // X축 회전 각도
float angleY = 0.0f;  // Y축 회전 각도
int prevX, prevY;     // 이전 마우스 좌표
bool isDragging = false; // 드래그 중인지 여부

void FlowerPot(float size) {//<- 원 뿔 자를까?
    GLdouble plane[] = {0.0, 0.0, -1, 0.7};  // mcs 기준 0.7 이하만 보임 
    glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, plane);
    glColor3f(0.545f, 0.271f, 0.075f);
    glutSolidCone(size, size + 1, 30, 30);
    //glutWireCone(size, size + 1, 30,30);
    glDisable(GL_CLIP_PLANE0);
}

void Soild() { //흙 표시 <- 어케해야되..감도 안 잡힘 (일단... 원뿔로.. 위에 살짝 잘린거 겹쳐? <- 보류!)
    GLdouble plane[] = { 0.0, 0.0, -1, 0.9 };  // mcs 기준
    glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, plane);
    //glColor3f(0.0, 0.0, 1.0); // 파랑 (임시)
    //glColor3f(0.545f, 0.271f, 0.075f);
    glColor3f(0.4f, 0.2f, 0.05f);
    glutSolidCone(0.4,  0.1, 30, 30);
    //glutWireCone(0.4,  0.2, 30, 30);
    glDisable(GL_CLIP_PLANE0);
}

void SideMainStem(float movx,float movy) { // 가지 중 메인 가지 < - 줄기 !!!!!!!!!!!! 아직 개발 안됨
    glPushMatrix();
    glTranslatef(movy, 0, 0.2); 
    glColor3f(0, 1, 0);
    //glTranslatef();
    glutSolidCone(0.02, movx, 30, 30);
    //glutWireCone(size, size + 1, 30,30);
    //glDisable(GL_CLIP_PLANE0);

    glPopMatrix();

}


void TomatoLeave() { // 잎 하나 (삼각형 형태)
    
    glColor3f(0.0f, 1.0f, 0.0f); // 초록색
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);  // 꼭짓점 1 (밑)
    glVertex3f(0.05f, 0.0f, 0.0f);  // 꼭짓점 2 (밑)
    glVertex3f(0.025f, 0.08f, 0.0f); // 꼭짓점 3 (끝)
    glEnd();
}

void TomatoLeaves() { // 잎 3개 
    glPushMatrix();
    glTranslatef(0.0f, 0.08f, 0.0f); // 토마토 위로 이동
    glRotatef(0, 0, 1, 0);
    TomatoLeave();
    glRotatef(120, 0, 1, 0);
    TomatoLeave();
    glRotatef(120, 0, 1, 0);
    TomatoLeave();
    glPopMatrix();

    
    

}


/*
void Tomato(float r, float slice_h=30, float slice_w=30) { // 토마토 객체
    glColor3f(1.0, 0.0, 0.0); // 빨강
    //glutWireSphere(r, slice_h, slice_w);   // Wireframe ( 임시 토마토 !)
    glutSolidSphere(r,slice_h,slice_w);  // 실Solid(속이 찬) 구 < - 조명 없어서 아직 원 처럼 보임 보류!

   
    TomatoLeaves();

}*/

/*

void TomatoDisplay( float movx, float movy, int cnt = 1) { //토마토 3개 묶음

    glPushMatrix();
    glTranslatef(movx,movy, 0); //최초 이동만 전역 부여 그 뒤는 지역 이동
    Tomato(0.11);
    if (cnt >= 2) {
        glTranslatef(0.12, 0.1, 0);
        glRotatef(30, 0, 0, 1);
 
        Tomato(0.1);
       
    }
    if (cnt == 3) { // 3개! (기본 ) // 수정 (1개 기본)
        glTranslatef(-0.04, 0, -0.2);
        Tomato(0.1);
        
    }
    glPopMatrix();
}



// 3. Small Leaf (토마토 꼭지 또는 가지 연결부 가리기용 잎)
void TomatoLeaf() {
    glColor3f(0.0f, 0.5f, 0.0f); // Green

    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.03f, 0.0f, 0.0f);
    glVertex3f(0.015f, 0.06f, 0.0f);
    glEnd();
}

// 3.5. Large Main Stem Leaf (주 줄기에 붙는 큰 잎) - 제거됨.

// 4. Tomato Leaves (토마토 열매 꼭지 잎)
void TomatoCapLeaves() { // 함수명 변경: 토마토에 붙는 잎임을 명확히 함
    glPushMatrix();
    glTranslatef(0.0f, 0.02f, 0.0f); // Move up slightly from the sphere's top

    // 3개의 작은 잎 배치
    for (int i = 0; i < 3; ++i) {
        glPushMatrix();
        glRotatef(i * 120.0f, 0, 1, 0);
        TomatoLeaf();
        glPopMatrix();
    }

    glPopMatrix();
}



// 5. Tomato object (토마토 열매 + 꼭지)
void Tomato(float r) {
    // Tomato (Solid Sphere)
    glColor3f(1.0, 0.0, 0.0); // Red
    glutSolidSphere(r, 30, 30);

    TomatoCapLeaves(); // Add cap leaves
}



// 6.5. Draw Leaves on Main Stem (주 줄기에 큰 잎 배치) - 제거됨.


// 7. Tomato Cluster Display (토마토 묶음 배치 - 가지 및 접합부 잎 포함)
// stem_h: Y축 줄기 높이, angle_y: Y축 회전 각도, count: 토마토 개수
void TomatoDisplay(float stem_h, float angle_y, int count = 2) {
    GLUquadric* quad = gluNewQuadric();
    glPushMatrix();

    // 1. Global positioning (주 줄기에 위치)
    glTranslatef(0.0f, stem_h, 0.0f);
    glRotatef(angle_y, 0.0f, 1.0f, 0.0f);

    // 2. Move out from the trunk (가지 시작 위치)
    float branch_start_offset = 0.045f;
    glTranslatef(0.0f, 0.0f, branch_start_offset);

    // Branch angle: Tilt up 10 degrees from horizontal
    glRotatef(10, 1.0f, 0.0f, 0.0f);

    // --- 2.1. Leaves to hide the joint (접합부 가리기용 잎) ---
    // 토마토 묶음의 접합부는 작은 잎으로 가리는 것이 자연스러움
    glColor3f(0.0f, 0.5f, 0.0f);
    glPushMatrix();
    glScalef(1.5, 1.5, 1.5); // 잎 크기 키우기
    glRotatef(90, 0, 1, 0); // 잎을 줄기 방향으로 세우기
    glTranslatef(0, 0.05, 0.0);

    // 잎 2개를 배치하여 연결부를 자연스럽게 감싸도록 함
    for (int i = 0; i < 2; ++i) {
        glPushMatrix();
        glRotatef(i * 180.0f + 45, 0, 0, 1);
        TomatoLeaf();
        glPopMatrix();
    }
    glPopMatrix();

    // 3. Draw the branch stem (가지 줄기)
    float branch_length = 0.12f;
    glPushMatrix();
    glColor3f(0.0f, 0.4f, 0.0f);
    // 가지 두께: 0.02
    gluCylinder(quad, 0.02, 0.02, branch_length, 10, 1);
    glPopMatrix();

    // 4. Tomato Cluster Group (토마토 열매 묶음)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, branch_length);
    glRotatef(50, 1.0f, 0.0f, 0.0f); // 아래로 처지게 회전

    // T1: Primary Tomato
    glPushMatrix();
    Tomato(0.11);
    glPopMatrix();

    if (count >= 2) {
        // T2: Side 1 (간격 확보)
        glPushMatrix();
        glTranslatef(0.08, -0.08, 0.08);
        glRotatef(15, 0, 1, 0);
        Tomato(0.1);
        glPopMatrix();
    }
    // 토마토 개수는 최대 2개로 제한

    glPopMatrix(); // End Tomato Cluster Group
    glPopMatrix(); // End TomatoDisplay Group
}

void Stem(float r, float h ,float slice=30) {
    GLUquadric* quad = gluNewQuadric();
    glColor3f(0, 0.6, 0);
    gluQuadricDrawStyle(quad, GLU_FILL);     // 채워진 원통
    gluCylinder(quad, r, r, h, slice, 1);
    gluDeleteQuadric(quad);
    

}



void Triangle(float width = 0.1f, float height = 0.1f, float range = 0.05f, float thickness = 0.01f) {
    // 앞면 꼭짓점
    float x1 = 0, y1 = 0, z1 = thickness / 2;
    float x2 = width, y2 = 0, z2 = thickness / 2;
    float x3 = width / 2 + range, y3 = height, z3 = thickness / 2;

    // 뒷면 꼭짓점 (z 반대로)
    float x1b = 0, y1b = 0, z1b = -thickness / 2;
    float x2b = width, y2b = 0, z2b = -thickness / 2;
    float x3b = width / 2 + range, y3b = height, z3b = -thickness / 2;

    // 앞면
    glBegin(GL_TRIANGLES);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glEnd();

    // 뒷면
    glBegin(GL_TRIANGLES);
    glVertex3f(x1b, y1b, z1b);
    glVertex3f(x2b, y2b, z2b);
    glVertex3f(x3b, y3b, z3b);
    glEnd();

    // 옆면 연결 (각 변을 사각형으로)
    glBegin(GL_QUADS);
    // 1번 변
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2b, y2b, z2b);
    glVertex3f(x1b, y1b, z1b);

    // 2번 변
    glVertex3f(x2, y2, z2);
    glVertex3f(x3, y3, z3);
    glVertex3f(x3b, y3b, z3b);
    glVertex3f(x2b, y2b, z2b);

    // 3번 변
    glVertex3f(x3, y3, z3);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1b, y1b, z1b);
    glVertex3f(x3b, y3b, z3b);
    glEnd();
}



void Leave(float size, float a, float b, float c) { // 식물 잎 <- 이건 어케 해야하냐....( 타원?  + 삼각형??)  아이공...아이구~~
    glPushMatrix();           // 현재 변환 상태 저장
    glScalef(a, b, c);
    glColor3f(0.0, 1.0, 0.0); // 초록
    glPushMatrix();

    glTranslatef(-0.1,0,-0.08);
    glRotatef(90, -1, 0, 0);
    Triangle();

    glTranslatef(0.1, 0, 0);
    Triangle();

    glRotatef(90, 0, 0, -1);
    glTranslatef(0.05, 0.1, 0);
    Triangle(0.1,0.1,0); // 잎 가운데 위치한 삼각형 ( 직각 ) 

    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, 0.08);
    glRotatef(90, 1, 0, 0);
    Triangle();
    glTranslatef(-0.1, 0, 0);
    Triangle();
    glPopMatrix();
    //glutWireSphere(size, 30, 30);
    
    //glutSolidSphere(size, 30, 30); 
    
    glPopMatrix();

}




//----- 재미나이 헬프 ------------------

void DrawSimpleLeaf(float length = 0.2f, float max_width = 0.1f, float z = 0.0f) {
    glBegin(GL_TRIANGLE_STRIP);

    // 1. 잎의 시작점 (줄기 쪽)
    glVertex3f(0.0f, 0.0f, z); // P0 (잎맥 중앙)
    glVertex3f(0.0f, 0.0f, z); // P1 (사실상 P0와 같음)

    // 2. 잎의 가장 넓은 부분
    glVertex3f(max_width / 2.0f, length * 0.4f, z); // P2 (오른쪽)
    glVertex3f(-max_width / 2.0f, length * 0.4f, z); // P3 (왼쪽)

    // 3. 잎의 중간 부분
    glVertex3f(max_width / 4.0f, length * 0.7f, z); // P4 (오른쪽)
    glVertex3f(-max_width / 4.0f, length * 0.7f, z); // P5 (왼쪽)

    // 4. 잎의 끝점
    glVertex3f(0.0f, length, z); // P6 (잎맥 끝)
    glVertex3f(0.0f, length, z); // P7 (사실상 P6와 같음)

    glEnd();
}



void Leave(float size, float a, float b, float c) {
    glPushMatrix();
    glScalef(a, b, c);
    glColor3f(0.1, 0.8, 0.1); // 초록색 (조금 더 자연스럽게)

    // 잎 그리기
    DrawSimpleLeaf(0.2f, 0.1f, 0.0f);

    // (선택) 잎맥 그리기 (GL_LINE_STRIP)
    glColor3f(0.0, 0.5, 0.0); // 더 진한 초록
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0f, 0.0f, 0.01f); // z값을 살짝 높여서 잎 위에 보이게
    glVertex3f(0.0f, 0.2f, 0.01f);
    glEnd();

    glPopMatrix();
}


void DrawLeaves() { // 잎
    glPushMatrix();
    glTranslatef(0.5, 1, 0);
    glRotatef(90, 0, 0, -1); // 잎 기본 형 오른쪽 방향
    glPushMatrix();
    //glRotatef()
    Leave(0.1, 2, 1, 1);   //잎
    glPopMatrix();
}


void SideStem(int size=0.1) {
    glPushMatrix();
    glColor3f(0, 1, 0);
    glutSolidCone(0.02, size, 30, 30);
    //glutWireCone(size, size + 1, 30,30);
    //glDisable(GL_CLIP_PLANE0);
    glPopMatrix();

}





void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 3, 3, 0, 0, 0, 0, 1, 0);

    glRotatef(angleX, 1, 0, 0);  // X축 회전
    glRotatef(angleY, 0, 1, 0);  // Y축 회전
    glPushMatrix();
    glTranslatef(0, 0.3, 0);
    glRotatef(90, 1, 0, 0);
    FlowerPot(0.45); // 화분
    //glPopMatrix();// 테스트용 지워야 함
    glRotatef(-90, 1, 0, 0);

    glRotatef(90, -1, 0, 0);
    Soild();
    glPopMatrix();
    TomatoDisplay(0.1,0.5,2); // 토마토
    TomatoDisplay(0.1,1.0,1);
    TomatoDisplay(-0.1,0.7,2);
    glPushMatrix();
    glRotatef(90, -1, 0, 0);
    Stem(0.02, 1.5, 30);
    glPopMatrix();
    //DrawLeaves();
    glutSwapBuffers();  // 더블 버퍼 사용 <- 자연스러운 애니메이션용!
}












void MyReshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLfloat)w / (GLfloat)h, 0.1, 100);
}

// 마우스 클릭 이벤트
void Mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true;
            prevX = x;
            prevY = y;
        }
        else {
            isDragging = false;
        }
    }
}

// 마우스 드래그 이벤트
void Motion(int x, int y) {
    if (isDragging) { 
        int dx = x - prevX;
        int dy = y - prevY;

        angleY += dx * 0.5f;  // 좌우 움직임 → Y축 회전
        angleX += dy * 0.5f;  // 상하 움직임 → X축 회전

        prevX = x;
        prevY = y;

        glutPostRedisplay(); // 화면 다시 그리기 요청
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 더블 버퍼 + 깊이 버퍼
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("cherry tomato");

    glEnable(GL_DEPTH_TEST); // 깊이 테스트 켜기
    glClearColor(0, 0, 0, 0);

    glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(Mouse);
    glutMotionFunc(Motion);

    glutMainLoop();
    return 0;
}

*/