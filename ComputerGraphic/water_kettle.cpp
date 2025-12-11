#include "water_kettle.h"
#include <iostream>
#include "glaux.h"
#include <algorithm>
#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLuint waterTextureID = -1;

float waterY = 0.0f;     // 물이 떨어지는 y 위치
bool isPouring = false;  // 기울여서 물이 나오는 상태

void loadTexture(void) {
	AUX_RGBImageRec *pTextureImage = auxDIBImageLoad( "texture/water3.bmp" );

    if( pTextureImage != NULL ) {
        glGenTextures( 1, &waterTextureID );   // 1: 로드할 텍스쳐 1개를 g_textureID에 저장

		glBindTexture( GL_TEXTURE_2D, waterTextureID );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		glTexImage2D( GL_TEXTURE_2D, 0, 3, pTextureImage->sizeX, pTextureImage->sizeY, 0,
				GL_RGB, GL_UNSIGNED_BYTE, pTextureImage->data );    // 0: mipmap단계, 원본 사용하겠다 3: RGB
	}

	if( pTextureImage ) {
		if( pTextureImage->data )
			free( pTextureImage->data );

		free( pTextureImage );
	}
}

void InitWaterKettleTextures() {
    loadTexture();
    if (waterTextureID == 0) {
        std::cerr << "Failed to load water texture (GLAUX)" << std::endl;
    }
}

void DrawRealWater(float bottomRadius, float topRadius, float height, int slices, float tiltAngle, float time) {
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, waterTextureID);
    glColor4f(0.4f, 0.6f, 0.9f, 0.4f);

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    if (tiltAngle > 10.0f) {
        glTranslatef(time * 0.05f, time * 0.03f, 0.0f);
    }
    glMatrixMode(GL_MODELVIEW);

    float angleStep = 2.0f * M_PI / slices;
    float radTilt = tiltAngle * (M_PI / 180.0f);
    float tiltFactor = tan(radTilt) * topRadius;

    // 동요 정도
    float agitation = std::min(1.0f, fabsf(tiltAngle) / 30.0f);

    // 1. 옆면 그리기 (Triangle Strip으로 윗점, 아랫점 연결)
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= slices; i++) {
        float angle = i * angleStep;
        float x = cos(angle);
        float y = sin(angle);

        // 텍스처 좌표 (가로로 반복)
        float u = (float)i / slices;

        // --- 아랫면 (고정) ---
        // 바닥은 평평하므로 z = 0
        glNormal3f(x, y, 0.0f);
        glTexCoord2f(u, 0.0f);
        glVertex3f(bottomRadius * x, bottomRadius * y, 0.0f);
        
        float waterHeightChange = -x * tiltFactor;

        // --- 윗면 (변형) ---
        float wave = 0.03f * agitation * sin(angle * 3.0f + time * 3.0f) * cos(angle * 2.0f + time);
        float currentH = height + waterHeightChange + wave;

        // 물이 병 밖으로 튀어나가지 않게 (선택사항)
        /*if (currentH > height + 0.3f) currentH = height + 0.3f;
        if (currentH < height - 0.25f) currentH = height - 0.25f;*/

        glNormal3f(x, y, 0.0f);
        glTexCoord2f(u, 1.0f);
        glVertex3f(topRadius * x, topRadius * y, currentH);
    }
    glEnd();

    // 2. 윗면 (수면) 뚜껑 닫기
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);

    glTexCoord2f(0.5f, 0.5f);
    glVertex3f(0.0f, 0.0f, height);

    for (int i = 0; i <= slices; i++) {
        float angle = -i * angleStep; // 텍스처가 뒤집히지 않게 반대로 돌림 (선택)
        float x = cos(angle);
        float y = sin(angle);

        float waterHeightChange = -x * tiltFactor;
        float wave = 0.03f * agitation * sin(angle * 3.0f + time * 3.0f) * cos(angle * 2.0f + time);
        float currentH = height + waterHeightChange + wave;

        // 텍스처 좌표 (원형 매핑)
        glTexCoord2f(0.5f + 0.5f * x, 0.5f + 0.5f * y);
        glVertex3f(topRadius * x, topRadius * y, currentH);
    }
    glEnd();

    // 3. 아랫면 (바닥) 뚜껑 닫기
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= slices; i++) {
        float angle = i * angleStep;
        glVertex3f(bottomRadius * cos(angle), bottomRadius * sin(angle), 0.0f);
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
}


void DrawWaterKettle(float x, float y, float z, float time, float tiltAngle) {
    glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);
	glPushMatrix();
	glTranslatef(x, y, z);
    glRotatef(tiltAngle, 0, 0, 1);

	glScalef(0.7f, 0.7f, 0.7f);

    GLUquadric* quad = gluNewQuadric();

    // 1. 밑받침
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 0.21f, 0.21f, 0.02f, 32, 32); // 받침대
    gluDisk(quad, 0.0f, 0.21f, 32, 1);             // 바닥
    glTranslatef(0.0f, 0.0f, 0.02f);
    gluDisk(quad, 0.0f, 0.21f, 32, 1);             // 윗면
    glPopMatrix();

    // 2. 물
    glPushMatrix();
    glTranslatef(0.0f, 0.03f, 0.0f); // 받침대 위로 살짝 띄움
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    GLdouble eqn[4] = { 0.0, 0.0, -1.0, 0.71 };
    glClipPlane(GL_CLIP_PLANE0, eqn);
    glEnable(GL_CLIP_PLANE0);
    
    DrawRealWater(0.18f, 0.16f, 0.6f, 32, tiltAngle, time);
    glDisable(GL_CLIP_PLANE0);

    glPopMatrix();
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_TEXTURE_2D);

    // 3. 유리 몸통
    GLfloat glass_diffuse[] = { 0.7f, 0.9f, 1.0f, 0.6f };
    GLfloat glass_specular[] = { 1.0f, 1.0f, 1.0f, 0.3f };
    GLfloat glass_ambient[] = { 0.1f, 0.1f, 0.1f, 0.3f };
    GLfloat glass_shine = 100.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, glass_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, glass_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, glass_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, glass_shine);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1, 1, 1, 0.15f);
    glPushMatrix();
    glTranslatef(0.0f, 0.02f, 0.0f); 
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    // 외곽 유리
    gluCylinder(quad, 0.2f, 0.17f, 0.73f, 32, 32);
    
    // 내부 유리 (두께)
    gluCylinder(quad, 0.19f, 0.16f, 0.73f, 32, 32);

    glPopMatrix();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    // 4. 뚜껑
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 0.75f, 0.0f); // 몸통 위
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 0.17f, 0.12f, 0.08f, 32, 32);
    glTranslatef(0.0f, 0.0f, 0.08f);
    gluDisk(quad, 0.0f, 0.12f, 32, 1); // 뚜껑 닫기
    glPopMatrix();

    // 5. 주둥이
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(-0.1f, 0.79f, 0.0f);  // 뚜껑 왼쪽
    glRotatef(-90.0f, 0.0f, 1.f, 0.0f);
    
    gluCylinder(quad, 0.04f, 0.01f, 0.10f, 16, 16);  // 끝 (뾰족)

    glPopMatrix();

    // 6. 손잡이
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.15f, 0.45f, 0.0f);   // 포트 오른쪽에 붙이기
   
    // 세로 막대
    glPushMatrix();
    glTranslatef(0.1f, 0.0f, 0.0f);
    glScalef(0.05f, 0.2f, 0.05f);      // 폭, 높이, 두께
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();
    // 아래 가로 막대
    glPushMatrix();
    glTranslatef(0.23f, 0.35f, 0.0f);
    glScalef(0.09f, 0.05f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 위 가로 막대
    glPushMatrix();
    glTranslatef(0.227f, 0.55f, 0.0f);
    glScalef(0.095f, 0.05f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 연결부
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();

    glTranslatef(0.185f, 0.385f, 0.0f);
    glRotatef(2.4f, 0, 0, 1);

    glScalef(0.001f, 0.73f, 0.05f);
    glutSolidCube(1.0f);

    glPopMatrix();
    // ==========================
      // 7. 기울이면 나오는 물줄기
      // ==========================
      // tiltAngle 기준으로 물 나오는지 여부 결정
    if (tiltAngle > 35.0f) {          // 임계각은 적당히 조절
        isPouring = true;
        waterY = std::min(1.0f, waterY + 0.05f);   // 점점 길게
    }
    else {
        waterY = std::max(0.0f, waterY - 0.08f);   // 원래대로 줄어듦
        if (waterY <= 0.0f)
            isPouring = false;
    }

    if (isPouring && waterY > 0.0f) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        glColor4f(0.4f, 0.6f, 1.0f, 0.8f);    // 물 색

        glPushMatrix();
        // 주둥이 위치/방향 재사용해서 그 위치에서 물 뽑아내기
        glTranslatef(-0.1f, 0.79f, 0.0f);
        glRotatef(-90.0f, 0.0f, 1.f, 0.0f);

        float streamLen = 0.25f + 0.4f * waterY; // 길이
        gluCylinder(quad, 0.015f, 0.01f, streamLen, 12, 1);
        glPopMatrix();

        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    glPopMatrix();   // 전체 포트 transform pop
    glPopAttrib();
    glPopMatrix();

    glPopAttrib();

    gluDeleteQuadric(quad);
}

void UpdateWaterKettle()
{
    waterTime += 0.01f;

    bool keepLifting = kettleSelected || (fabs(kettleAngle) > 2.0f);

    // 선택된 상태라면 위로
    if (keepLifting)
        kettleLift = std::min(0.5f, kettleLift + 0.02f);
    else
        kettleLift = std::max(0.0f, kettleLift - 0.02f);
}