//#include <GL/glew.h>
//#include <GL/freeglut.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>
//#include <GL/glut.h>
//
//#include <GL/glut.h>
//
//float angleX = 0.0f;  // X축 회전 각도
//float angleY = 0.0f;  // Y축 회전 각도
//int prevX, prevY;     // 이전 마우스 좌표
//bool isDragging = false; // 드래그 중인지 여부
//
//void FlowerPot(float size) {//<- 원 뿔 자를까?
//    GLdouble plane[] = {0.0, 0.0, -1, 0.7};  // mcs 기준 0.6 이하만 보임 
//    glEnable(GL_CLIP_PLANE0);
//    glClipPlane(GL_CLIP_PLANE0, plane);
//    glColor3f(0.545f, 0.271f, 0.075f);
//    glutWireCone(size, size + 1, 30,30);
//    glDisable(GL_CLIP_PLANE0);
//}
//
//void Soild() { //흙 표시 <- 어케해야되..감도 안 잡힘 (일단... 원뿔로.. 위에 살짝 잘린거 겹쳐놓음)
//    GLdouble plane[] = { 0.0, 0.0, -1, 0.9 };  // mcs 기준 0.6 이하만 보임 
//    glEnable(GL_CLIP_PLANE0);
//    glClipPlane(GL_CLIP_PLANE0, plane);
//    glColor3f(0.0, 0.0, 1.0); // 파랑
//    //glColor3f(0.545f, 0.271f, 0.075f);
//
//    //glutSolidCone(0.3,  0.3, 30, 30);
//    glutWireCone(0.4,  0.2, 30, 30);
//    glDisable(GL_CLIP_PLANE0);
//}
//
//
//void Tomato(float r, float slice_h=30, float slice_w=30) { // 토마토 객체
//    glColor3f(1.0, 0.0, 0.0); // 빨강
//    //glutSolidSphere(r,slice_h,slice_w);  // 실Solid(속이 찬) 구 < - 조명 없어서 아직 원 처럼 보임 보류!
//
//    glutWireSphere(r, slice_h, slice_w);   // Wireframe ( 임시 토마토 !)
//
//}
//
//void Plant(float size,float a, float b, float c) { // 식물 잎 <- 이건 어케 해야하냐....( 타원? )
//    glPushMatrix();           // 현재 변환 상태 저장
//    glScalef(a, b, c);   
//    glColor3f(0.0, 1.0, 0.0); // 초록// x=a, y=b, z=c로 스케일
//    glutSolidSphere(size, 30, 30);  // 구를 그리되, 1반지름 구를 변형
//    glPopMatrix();
//}
//
//void TomatoDisplay( float movx, float movy, int cnt = 3) { //토마토 3개 묶음
//    glPushMatrix();
//    glTranslatef(movx,movy, 0); //최초 이동만 전역 부여 그 뒤는 지역 이동
//    Tomato(0.11);
//    glTranslatef(-0.04,0, -0.2);
//    Tomato(0.1);
//    if (cnt == 3) {
//        glRotatef(30, 0, -1, 0);
//        glTranslatef(0.22, 0, 0);
//        Tomato(0.1);
//    }
//    glPopMatrix();
//}
//
//void Stem(float r, float h ,float slice=30) {
//    GLUquadric* quad = gluNewQuadric();
//    glColor3f(0, 1, 0);
//    gluQuadricDrawStyle(quad, GLU_FILL);     // 채워진 원통
//    gluCylinder(quad, r, r, h, slice, 1);
//    gluDeleteQuadric(quad);
//}
//
//
//void MyDisplay() {
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(0, 3, 3, 0, 0, 0, 0, 1, 0);
//
//    glRotatef(angleX, 1, 0, 0);  // X축 회전
//    glRotatef(angleY, 0, 1, 0);  // Y축 회전
//    glPushMatrix();
//    glTranslatef(0, 0.3, 0);
//    glRotatef(90, 1, 0, 0);
//    FlowerPot(0.45); // 화분
//    glRotatef(-90, 1, 0, 0);
//    glTranslatef(0, -0.1, 0);
//    glRotatef(90, -1, 0, 0);
//    Soild();
//    glPopMatrix();
//
//    TomatoDisplay(0.4,0.5); // 토마토
//    TomatoDisplay(0.2,1.0,2);
//    TomatoDisplay(-0.3,0.7);
//    glPushMatrix();
//    glRotatef(90, -1, 0, 0);
//    Stem(0.02, 1.5, 30);
//    glPopMatrix();
//    /*glPushMatrix();
//    glTranslatef(0.5, 1, 0);
//    Plant(0.1,3, 1, 0.1);   //잎
//    glPopMatrix();*/
//    glFlush();
//    glutSwapBuffers();  // 더블 버퍼 사용 <- 자연스러운 애니메이션용!
//}
//
//
//
//
//
//
//
//
//
//
//void MyReshape(int w, int h) {
//    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(45, (GLfloat)w / (GLfloat)h, 0.1, 100);
//}
//
//// 마우스 클릭 이벤트
//void Mouse(int button, int state, int x, int y) {
//    if (button == GLUT_LEFT_BUTTON) {
//        if (state == GLUT_DOWN) {
//            isDragging = true;
//            prevX = x;
//            prevY = y;
//        }
//        else {
//            isDragging = false;
//        }
//    }
//}
//
//// 마우스 드래그 이벤트
//void Motion(int x, int y) {
//    if (isDragging) { 
//        int dx = x - prevX;
//        int dy = y - prevY;
//
//        angleY += dx * 0.5f;  // 좌우 움직임 → Y축 회전
//        angleX += dy * 0.5f;  // 상하 움직임 → X축 회전
//
//        prevX = x;
//        prevY = y;
//
//        glutPostRedisplay(); // 화면 다시 그리기 요청
//    }
//}
//
//int main(int argc, char** argv)
//{
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 더블 버퍼 + 깊이 버퍼
//    glutInitWindowSize(500, 500);
//    glutInitWindowPosition(0, 0);
//    glutCreateWindow("Mouse Rotation Teapot");
//
//    glEnable(GL_DEPTH_TEST); // 깊이 테스트 켜기
//    glClearColor(0, 0, 0, 0);
//
//    glutDisplayFunc(MyDisplay);
//    glutReshapeFunc(MyReshape);
//    glutMouseFunc(Mouse);
//    glutMotionFunc(Motion);
//
//    glutMainLoop();
//    return 0;
//}
