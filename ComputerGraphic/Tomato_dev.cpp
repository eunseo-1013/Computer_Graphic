#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>



/*


// 마우스 회전 관련 전역 변수
float angleX = 0.0f;
float angleY = 0.0f;
int prevX, prevY;
bool isDragging = false;

// GLU Quadric object declaration
GLUquadric* quad = NULL;

//---------------------------------------------------------
// 1. 화분 및 흙
//---------------------------------------------------------

void FlowerPot(float size) {
	GLdouble plane[] = { 0.0, 0.0, -1, 0.7 };
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, plane);
	glColor3f(0.545f, 0.271f, 0.075f); // 화분 갈색
	glutSolidCone(size, size + 1, 30, 30);
	glDisable(GL_CLIP_PLANE0);
}

void Soild() {
	GLdouble plane[] = { 0.0, 0.0, -1, 0.9 };
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, plane);
	glColor3f(0.4f, 0.2f, 0.05f); // 흙 색상
	glutSolidCone(0.4, 0.1, 30, 30);
	glDisable(GL_CLIP_PLANE0);
}

//---------------------------------------------------------
// 2. 줄기 및 잎
//---------------------------------------------------------

void Stem(float r, float h, float slice = 30) {
	if (!quad) quad = gluNewQuadric();
	glColor3f(0.0f, 0.6f, 0.0f); // 줄기 색
	gluQuadricDrawStyle(quad, GLU_FILL);
	gluCylinder(quad, r, r, h, slice, 1);
}



void TomatoLeaf() {
	// 색상 진하게 조정
	glRotatef(90, -1, 0, 0);
	glColor3f(0.0f, 0.4f, 0.0f);
	glBegin(GL_TRIANGLES);
	// 잎에 약간의 두께(깊이)를 주어 얇은 평면으로 인한 깜빡임을 방지
	glVertex3f(0.0f, 0.0f, 0.005f);
	glVertex3f(0.03f, 0.0f, 0.005f);
	// 높이 조정
	glVertex3f(0.005f, 0.05f, 0.005f);
	glEnd();
}

void TomatoCapLeaves(float r) {
	// 잎이 구체 위로 더 잘 보이도록 이동
	glTranslatef(0.0f, r + 0.01, 0.0f);
	for (int i = 0; i < 3; ++i) {
		glPushMatrix();
		glRotatef(i * 120.0f, 0, 1, 0);
		TomatoLeaf();
		glPopMatrix();
	}
}

void Tomato(float r) {
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0); // 빨강
	glutSolidSphere(r, 30, 30);
	TomatoCapLeaves(r);
	glPopMatrix();
}

void DrawLeaflet(float width, float length, float tipOffset) {
	// 잎 조각 하나 (길쭉한 타원형 또는 삼각형 형태)
	glBegin(GL_TRIANGLES);

	// 색상 설정 (밝은 녹색)
	glColor3f(0.1f, 0.6f, 0.1f);

	// 꼭짓점 3개로 구성된 잎 조각
	// 1. 줄기 쪽 넓은 부분 (왼쪽)
	glVertex3f(-width / 2.0f, 0.0f, 0.0f);
	// 2. 줄기 쪽 넓은 부분 (오른쪽)
	glVertex3f(width / 2.0f, 0.0f, 0.0f);
	// 3. 잎 끝 (tipOffset을 이용해 끝을 뾰족하게)
	glVertex3f(tipOffset, length, 0.0f);

	glEnd();
}

void DrawTomatoLeaf(float len, float wid) {

	// 잎의 색상 (밝지만 자연스러운 풀잎색: R=0.1, G=0.9, B=0.1)
	glColor3f(0.1f, 0.9f, 0.1f);

	float length = len; // 잎의 길이
	float width = wid;
	float z_bulge = 0.02f; // 중앙이 부풀어 오른 정도

	// ----------------------------------------------------
	// 1. 잎의 왼쪽 반쪽 (기존 코드와 동일)
	// ----------------------------------------------------
	glBegin(GL_QUAD_STRIP);

	// 시작점
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-width / 4.0f, 0.0f, 0.0f);

	// 중간 (가장 넓고 부푼 곳)
	glVertex3f(0.0f, length * 0.4f, z_bulge);
	glVertex3f(-width / 2.0f, length * 0.4f, 0.0f);

	// 끝으로 가늘어지는 부분
	glVertex3f(0.0f, length * 0.8f, z_bulge / 2.0f);
	glVertex3f(-width / 4.0f, length * 0.8f, 0.0f);

	// 잎의 끝점 (뾰족하게 마무리)
	glVertex3f(0.0f, length, 0.0f);
	glVertex3f(0.0f, length, 0.0f);

	glEnd();

	// ----------------------------------------------------
	// 2. 잎의 오른쪽 반쪽 (대칭 구조)
	// ----------------------------------------------------
	glBegin(GL_QUAD_STRIP);

	// 시작점 (Z값은 왼쪽과 동일하거나 0으로 유지)
	glVertex3f(0.0f, 0.0f, 0.0f); // 중앙 
	glVertex3f(width / 4.0f, 0.0f, 0.0f); // **오른쪽 옆**

	// 중간 (가장 넓고 부푼 곳)
	glVertex3f(0.0f, length * 0.4f, z_bulge); // 중앙 부풀음
	glVertex3f(width / 2.0f, length * 0.4f, 0.0f); // **오른쪽 옆**

	// 끝으로 가늘어지는 부분
	glVertex3f(0.0f, length * 0.8f, z_bulge / 2.0f);
	glVertex3f(width / 4.0f, length * 0.8f, 0.0f); // **오른쪽 옆**

	// 잎의 끝점
	glVertex3f(0.0f, length, 0.0f);
	glVertex3f(0.0f, length, 0.0f);

	glEnd();

	// ----------------------------------------------------
	// 3. 중앙 잎맥 (Lines)
	// ----------------------------------------------------
	glColor3f(0.0f, 0.4f, 0.0f); // 진한 녹색
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, length, 0.0f);
	glEnd();
}

void TomatoDisplay(float stem_h, float angle_y, int count = 2) {
	glPushMatrix();
	// 1. 위치 및 나선형 회전
	glTranslatef(0.0f, stem_h, 0.0f);
	glRotatef(angle_y, 0.0f, 1.0f, 0.0f);

	float branch_start_offset = 0.03f;
	glTranslatef(0.0f, 0.2f, branch_start_offset);
	glPushMatrix(); // 잎 한 쌍을 그리기 위한 전체 변환 저장
	// 잎의 기본 방향과 위치 설정
	glRotatef(90, 1, 0, 0);
	glTranslatef(0.02, 0, -0.02);

	// 잎 1: Y축(세로축) 기준 15도 회전
	glPushMatrix();
	//glRotatef(60.0f, 0.0f, -1.0f, 0.0f);
	DrawTomatoLeaf(0.13,0.1);
	glPopMatrix();

	// 잎 2: Y축(세로축) 기준 -15도 회전 (반대 방향)
	glPushMatrix();
	glRotatef(-60.0f, 0.0f, 0.0f, 1.0f);
	DrawTomatoLeaf(0.15,0.13);
	glPopMatrix();

	glPopMatrix(); // 잎 변환 스택 복원
	glRotatef(20, 1.0f, 0.0f, 0.0f); // 가지를 아래로 살짝 기울임

	// 2. 가지 줄기 (길이 연장)
	float branch_length = 0.18f; // 가지 길이를 늘려 줄기에서 분리되게 함
	glPushMatrix();

	glColor3f(0.0f, 0.4f, 0.0f);
	if (!quad) quad = gluNewQuadric();
	gluCylinder(quad, 0.012, 0.01, branch_length, 10, 1);
	glPopMatrix();

	// 3. 토마토 열매 묶음
	glPushMatrix();

	glTranslatef(0.0f, -0.1f, branch_length); // 가지 끝으로 이동
	glRotatef(-40, 1, 0, 0);
	glRotatef(30, 1.0f, 0.0f, 0.0f);

	// T1: Primary Tomato
	glPushMatrix();
	Tomato(0.11);
	glPopMatrix();

	if (count >= 2) {
		// T2: Side Tomato
		glPushMatrix();
		glTranslatef(0.08, -0.05, 0.08);
		glRotatef(15, 0, 1, 0);
		Tomato(0.09);
		glPopMatrix();
	}

	glPopMatrix();
	glPopMatrix();
}







void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 3, 3, 0, 0, 0, 0, 1, 0);

	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);

	// --- 화분 및 흙 (위치 조정) ---
	glPushMatrix();
	glTranslatef(0, -0.2, 0); // Y축 이동 값을 음수로 변경하여 아래로 내림
	glRotatef(90, 1, 0, 0);
	FlowerPot(0.45);
	glRotatef(-90, 1, 0, 0);
	glRotatef(90, -1, 0, 0);
	Soild();
	glPopMatrix();

	// --- 줄기 그리기 ---
	glPushMatrix();
	glRotatef(90, -1, 0, 0);
	// 화분 위치를 내린 만큼 줄기 시작점도 조정
	glTranslatef(0.0, 0, -0.5);
	Stem(0.02, 1.5, 30);
	glPopMatrix();

	// --- 방울토마토 묶음 배치 (나선형, 3개 묶음으로 축소) ---
	float initial_height = 0.1f;
	float height_step = 0.3f;
	float angle_step = 70.0f; // 나선형 간격 설정
	int total_clusters = 4;
	float angle_step2 = 80;

	glPushMatrix();
	glRotatef(angle_step2, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	DrawTomatoLeaf(0.08, 0.04);
	glPopMatrix();
	for (int i = 0; i < total_clusters; i++) {
		float current_height = initial_height + (i * height_step);
		float current_angle = i * angle_step;
		int count = 0;
		if (i < 2) {
			count = 2;
		}
		else {
			count = 1;
		}
		if (i == 3) {
			count = 2;
			current_height = initial_height + (height_step);
		}

		TomatoDisplay(current_height, current_angle, count);
	}


	glutSwapBuffers();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)w / (GLfloat)h, 0.1, 100);
}

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

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Cherry Tomato Plant (Spiral)");

	// 깊이 테스트만 활성화 (조명 없음)
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0);

	if (!quad) quad = gluNewQuadric();

	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	glutMainLoop();
	return 0;
} 

*/