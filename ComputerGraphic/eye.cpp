
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include "Camera.h"
#include "control.h"


void SetupFog()
{
    glEnable(GL_FOG);

    GLfloat fogColor[] = { 0.9f, 0.9f, 0.9f, 1.0f }; // 뿌연 흰색
    glFogfv(GL_FOG_COLOR, fogColor);

    glFogi(GL_FOG_MODE, GL_LINEAR);  // 선형 안개 (제일 자연스러움)

    glFogf(GL_FOG_START, 1.0f);      // 이 거리부터 뿌옇게
    glFogf(GL_FOG_END, 6.0f);        // 이 거리에서 거의 안 보임
}



GLuint gNoiseTex0 = 0;

GLuint gNoiseTex1 = 0;

GLuint gNoiseTex2 = 0;





void DrawNoiseOverlay(float alpha,int num)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    if (num == 0) {
        glBindTexture(GL_TEXTURE_2D, gNoiseTex0);
    }
    else if (num == 1) {
        glBindTexture(GL_TEXTURE_2D, gNoiseTex1);
    }
    else if (num == 2) {
        glBindTexture(GL_TEXTURE_2D, gNoiseTex2);
    }
   
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // 화면 좌표계로 전환
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 약간 회백색 + 투명도
    glColor4f(1.0f, 1.0f, 1.0f, alpha);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(windowWidth, 0);
    glTexCoord2f(1, 1); glVertex2f(windowWidth, windowHeight);
    glTexCoord2f(0, 1); glVertex2f(0, windowHeight);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}
