#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

// --- [수정 1] POINT 구조체 대체 ---
struct MousePoint {
    int x;
    int y;
};

float g_fDistance = -0.5f; // 모델이 잘 보이도록 거리 조정
float g_fSpinX = 0.0f;
float g_fSpinY = 0.0f;

static MousePoint ptLastMousePosit;
static MousePoint ptCurrentMousePosit;
static bool bMousing = false;

std::vector<glm::vec3> vertices;
std::vector<glm::ivec3> faces;

// ★ 경로 확인 필수
char filepath[1000] = "C:/Users/eunse/Downloads/Data/Data/bunny/bun_zipper.ply";


bool LoadPly(const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::ivec3>& out_faces)
{
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file: %s\n", path);
        return false;
    }

    out_vertices.clear();
    out_faces.clear();

    int nbVertices = 0;
    int nbFaces = 0;
    char buffer[128];

    while (fscanf(file, "%s", buffer) != EOF) {
        if (strcmp(buffer, "vertex") == 0) {
            fscanf(file, "%d", &nbVertices);
        }
        else if (strcmp(buffer, "face") == 0) {
            fscanf(file, "%d", &nbFaces);
        }
        else if (strcmp(buffer, "end_header") == 0) {
            break;
        }
    }

    for (int i = 0; i < nbVertices; i++) {
        float x, y, z;
        fscanf(file, "%f %f %f", &x, &y, &z);
        out_vertices.push_back(vec3(x, y, z));
    }

    for (int i = 0; i < nbFaces; i++) {
        int n, v1, v2, v3;
        fscanf(file, "%d %d %d %d", &n, &v1, &v2, &v3);
        out_faces.push_back(ivec3(v1, v2, v3));
    }
    fclose(file);
    return true;
}


void DrawWireSurface(std::vector < glm::vec3 >& vertices,
    std::vector < glm::ivec3 >& faces)
{
    glBegin(GL_LINES);
    glColor3f(0.7f, 0.7f, 0.7f);

    for (const auto& f : faces)
    {
        if (f.x >= vertices.size() || f.y >= vertices.size() || f.z >= vertices.size()) continue;

        glm::vec3 v0 = vertices[f.x];
        glm::vec3 v1 = vertices[f.y];
        glm::vec3 v2 = vertices[f.z];

        glVertex3f(v0.x, v0.y, v0.z);
        glVertex3f(v1.x, v1.y, v1.z);

        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);

        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v0.x, v0.y, v0.z);
    }
    glEnd();
}


void MyMouse(int button, int state, int x, int y) {
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            ptLastMousePosit.x = ptCurrentMousePosit.x = x;
            ptLastMousePosit.y = ptCurrentMousePosit.y = y;
            bMousing = true;
        }
        else
            bMousing = false;
        break;
    }
    glutPostRedisplay();
}

void MyMotion(int x, int y) {
    ptCurrentMousePosit.x = x;
    ptCurrentMousePosit.y = y;

    if (bMousing)
    {
        g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
        g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
        glutPostRedisplay();
    }

    ptLastMousePosit.x = ptCurrentMousePosit.x;
    ptLastMousePosit.y = ptCurrentMousePosit.y;
}


// --- [수정 3] render 함수 로직 변경 ---
void render(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 검은 배경
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
   
    // 1. 카메라 설정 (LookAt) - 여기 있어야 초기화되지 않음
    gluLookAt(0.0, 0.0, 0.5,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);

    // 2. 물체 이동/회전
    
    glTranslatef(0.0f, 0.0f, g_fDistance);
    glRotatef(g_fSpinX, 1.0f, 0.0f, 0.0f); // X축 회전
    glRotatef(g_fSpinY, 0.0f, 1.0f, 0.0f); // Y축 회전
    glScalef(5, 5, 5);
    // 3. 그리기
    DrawWireSurface(vertices, faces);

    glutSwapBuffers();
}


void MyReshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.01, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("234562_hw6");

    // 파일 로드
    if (!LoadPly(filepath, vertices, faces)) {
        printf("파일 로드 실패!\n");
    }

    glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화

    glutDisplayFunc(render);
    glutReshapeFunc(MyReshape);
    glutMouseFunc(MyMouse);
    glutMotionFunc(MyMotion);

    glutMainLoop();
    return 0;
}