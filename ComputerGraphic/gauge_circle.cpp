// gauge_click_12.c
#include <GL/glut.h>
#include <math.h>




float g_value = 0.0f;   // 0.0 ~ 1.0 사이 게이지 값

void drawCircularGauge(float cx, float cy,
    float radius, float thickness,
    float value)
{
    if (value < 0.0f) value = 0.0f;
    if (value > 1.0f) value = 1.0f;

    const int segments = 64;
    const float innerR = radius - thickness;
    const float outerR = radius;
    const float PI = 3.1415926f;

    // 회색 배경 링
    glColor3f(0.2f, 0.2f, 0.2f);

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float t = (float)i / (float)segments;   // 0 ~ 1
        float angle = t * 2.0f * PI;           

        float xOuter = cx + cosf(angle) * outerR;
        float yOuter = cy + sinf(angle) * outerR;
        float xInner = cx + cosf(angle) * innerR;
        float yInner = cy + sinf(angle) * innerR;

        glVertex2f(xOuter, yOuter);
        glVertex2f(xInner, yInner);
    }
    glEnd();


    // 파란색 원! 
    glColor3f(0.0f, 0.7f, 1.0f);

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float t = (float)i / (float)segments;  // 0 ~ 1 고정
        if (t > value) break;                  // value 만큼

        //시계방향 채우기
        float angle = (PI / 2.0f) - t * 2.0f * PI;

        float xOuter = cx + cosf(angle) * outerR;
        float yOuter = cy + sinf(angle) * outerR;
        float xInner = cx + cosf(angle) * innerR;
        float yInner = cy + sinf(angle) * innerR;

        glVertex2f(xOuter, yOuter);
        glVertex2f(xInner, yInner);
    }
    glEnd();
}




/*
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

  
    drawCircularGauge(400.0f, 300.0f, 150.0f, 30.0f, g_value);

    glutSwapBuffers();
}


void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        g_value += 0.1f;
        if (g_value > 1.0f) g_value = 1.0f;  // 최대 1.0

        glutPostRedisplay();
    }

  
    /*
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        g_value -= 0.1f;
        if (g_value < 0.0f) g_value = 0.0f;
        glutPostRedisplay();
    }
    */ /*
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Circular Gauge - 12 o'clock Start, Click to Increase");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
*/

