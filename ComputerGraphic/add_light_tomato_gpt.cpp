#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

// 마우스 회전 관련 전역 변수
float angleX = 0.0f;
float angleY = 0.0f;
int prevX, prevY;
bool isDragging = false;

// GLU Quadric object declaration
GLUquadric* quad = NULL;

// ----------------------------------------------------
// 0. 조명 및 재질 설정 함수
// ----------------------------------------------------
/**
 * 물체의 재질 속성을 설정합니다.
 * @param r, g, b: 기본 색상 및 Diffuse 속성
 * @param shine: 광택 강도 (0.0 ~ 128.0)
 */
void setMaterial(float r, float g, float b, float shine) {
	// Ambient: 주변광에 반응하는 색상 (Diffuse의 20%로 설정)
	GLfloat mat_ambient[] = { r * 0.2f, g * 0.2f, b * 0.2f, 1.0f };
	// Diffuse: 산란광에 반응하는 색상 (기본 색상)
	GLfloat mat_diffuse[] = { r, g, b, 1.0f };
	// Specular: 반사광에 반응하는 색상 (흰색 하이라이트)
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	// GL_COLOR_MATERIAL을 활성화했으므로 glColor3f로 설정하면 AMBIENT와 DIFFUSE도 함께 적용됩니다.
	glColor3f(r, g, b);
}


//---------------------------------------------------------
// 1. 화분 및 흙
//---------------------------------------------------------

void FlowerPot(float size) {
	GLdouble plane[] = { 0.0, 0.0, -1, 0.7 };
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, plane);

	// setMaterial 적용 (화분 갈색, 낮은 광택)
	setMaterial(0.545f, 0.271f, 0.075f, 10.0f);
	glutSolidCone(size, size + 1, 30, 30);

	glDisable(GL_CLIP_PLANE0);
}

void Soild() {
	GLdouble plane[] = { 0.0, 0.0, -1, 0.9 };
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, plane);

	// ----------------------------------------------------
	// 조명 비활성화 (요청 사항)
	glDisable(GL_LIGHTING);
	// ----------------------------------------------------

	glColor3f(0.4f, 0.2f, 0.05f); // 흙 색상 (평면 색상)
	glutSolidCone(0.4, 0.1, 30, 30);

	// ----------------------------------------------------
	// 조명 다시 활성화
	glEnable(GL_LIGHTING);
	// ----------------------------------------------------

	glDisable(GL_CLIP_PLANE0);
}

//---------------------------------------------------------
// 2. 줄기 및 잎
//---------------------------------------------------------

void Stem(float r, float h, float slice = 30) {
	if (!quad) quad = gluNewQuadric();

	// setMaterial 적용 (줄기 색, 보통 광택)
	setMaterial(0.0f, 0.6f, 0.0f, 20.0f);

	gluQuadricDrawStyle(quad, GLU_FILL);
	gluCylinder(quad, r, r, h, slice, 1);
}



void TomatoLeaf() {
	// setMaterial 적용 (진한 녹색 잎)
	setMaterial(0.0f, 0.4f, 0.0f, 30.0f);

	glRotatef(90, -1, 0, 0);
	glBegin(GL_TRIANGLES);

	// 법선 벡터 추가 (잎 평면에 수직)
	glNormal3f(0.0f, 0.0f, 1.0f);

	glVertex3f(0.0f, 0.0f, 0.005f);
	glVertex3f(0.03f, 0.0f, 0.005f);
	glVertex3f(0.005f, 0.05f, 0.005f);
	glEnd();
}

void TomatoCapLeaves(float r) {
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

	// setMaterial 적용 (빨강, 높은 광택)
	setMaterial(1.0, 0.0, 0.0, 90.0f);

	glutSolidSphere(r, 30, 30);
	TomatoCapLeaves(r);
	glPopMatrix();
}

void DrawTomatoLeaf(float len, float wid) {
	// setMaterial 적용 (밝고 자연스러운 풀잎색)
	setMaterial(0.1f, 0.9f, 0.1f, 40.0f);

	float length = len;
	float width = wid;
	float z_bulge = 0.02f;

	// ----------------------------------------------------
	// 1. 잎의 왼쪽 반쪽 
	// ----------------------------------------------------
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0.0f, 0.0f, 1.0f); // 법선 벡터 설정

	// 시작점
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-width / 4.0f, 0.0f, 0.0f);

	// 중간
	glVertex3f(0.0f, length * 0.4f, z_bulge);
	glVertex3f(-width / 2.0f, length * 0.4f, 0.0f);

	// 끝으로 가늘어지는 부분
	glVertex3f(0.0f, length * 0.8f, z_bulge / 2.0f);
	glVertex3f(-width / 4.0f, length * 0.8f, 0.0f);

	// 잎의 끝점 
	glVertex3f(0.0f, length, 0.0f);
	glVertex3f(0.0f, length, 0.0f);

	glEnd();

	// ----------------------------------------------------
	// 2. 잎의 오른쪽 반쪽 
	// ----------------------------------------------------
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0.0f, 0.0f, 1.0f); // 법선 벡터 설정

	// 시작점 
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(width / 4.0f, 0.0f, 0.0f);

	// 중간 
	glVertex3f(0.0f, length * 0.4f, z_bulge);
	glVertex3f(width / 2.0f, length * 0.4f, 0.0f);

	// 끝으로 가늘어지는 부분
	glVertex3f(0.0f, length * 0.8f, z_bulge / 2.0f);
	glVertex3f(width / 4.0f, length * 0.8f, 0.0f);

	// 잎의 끝점
	glVertex3f(0.0f, length, 0.0f);
	glVertex3f(0.0f, length, 0.0f);

	glEnd();

	// ----------------------------------------------------
	// 3. 중앙 잎맥 (Lines)
	// ----------------------------------------------------
	setMaterial(0.0f, 0.4f, 0.0f, 10.0f); // 진한 녹색
	glBegin(GL_LINES);
	glNormal3f(0.0f, 0.0f, 1.0f);
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

	// 잎 1: Y축(세로축) 기준 회전
	glPushMatrix();
	DrawTomatoLeaf(0.13, 0.1);
	glPopMatrix();

	// 잎 2: Y축(세로축) 기준 회전 (반대 방향)
	glPushMatrix();
	glRotatef(-60.0f, 0.0f, 0.0f, 1.0f);
	DrawTomatoLeaf(0.15, 0.13);
	glPopMatrix();

	glPopMatrix(); // 잎 변환 스택 복원
	glRotatef(20, 1.0f, 0.0f, 0.0f); // 가지를 아래로 살짝 기울임

	// 2. 가지 줄기 (길이 연장)
	float branch_length = 0.18f;
	glPushMatrix();

	// 가지 줄기 재질 설정
	setMaterial(0.0f, 0.4f, 0.0f, 20.0f);
	if (!quad) quad = gluNewQuadric();
	gluCylinder(quad, 0.012, 0.01, branch_length, 10, 1);
	glPopMatrix();

	// 3. 토마토 열매 묶음
	glPushMatrix();

	glTranslatef(0.0f, -0.1f, branch_length);
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

	// ----------------------------------------------------
	// 광원 위치 설정 (월드 좌표계에 고정된 평행 광원)
	// ----------------------------------------------------
	GLfloat light_position[] = { 1.0f, 5.0f, 5.0f, 0.0f }; // W=0.0: 평행 광원
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	// ----------------------------------------------------

	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);

	// --- 화분 및 흙 (위치 조정) ---
	glPushMatrix();
	glTranslatef(0, -0.2, 0);
	glRotatef(90, 1, 0, 0);
	FlowerPot(0.45);
	glRotatef(-90, 1, 0, 0);
	glRotatef(90, -1, 0, 0);
	Soild(); // **여기에서만 잠시 조명이 꺼집니다.**
	glPopMatrix();

	// --- 줄기 그리기 ---
	glPushMatrix();
	glRotatef(90, -1, 0, 0);
	glTranslatef(0.0, 0, -0.5);
	Stem(0.02, 1.5, 30);
	glPopMatrix();

	// --- 방울토마토 묶음 배치 ---
	float initial_height = 0.1f;
	float height_step = 0.3f;
	float angle_step = 70.0f;
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
	glutCreateWindow("Cherry Tomato Plant (Spiral) with Selective Lighting");

	// ----------------------------------------------------
	// 조명 시스템 초기 활성화
	// ----------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING); // 조명 시스템 활성화
	glEnable(GL_LIGHT0);   // 0번 광원 활성화
	glEnable(GL_NORMALIZE); // 모델 변환 시 법선 벡터 자동 정규화

	// 광원 속성 설정 (흰색 광원)
	GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// ----------------------------------------------------

	glClearColor(0, 0, 0, 0);

	if (!quad) quad = gluNewQuadric();

	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	glutMainLoop();
	return 0;
}