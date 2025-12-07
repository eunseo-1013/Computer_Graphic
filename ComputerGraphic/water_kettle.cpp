#include "water_kettle.h"
#include <iostream>
#include "glaux.h"
#include <algorithm>

GLuint waterTextureID = -1;

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, waterTextureID);
    gluQuadricTexture(quad, GL_TRUE);

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glTranslatef(time * 0.05f, time * 0.03f, 0.0f);  // 흐르는 물처럼 보임
    glMatrixMode(GL_MODELVIEW);

    glColor4f(0.4f, 0.6f, 1.0f, 0.5f);
    glPushMatrix();
    glTranslatef(0.0f, 0.03f, 0.0f); // 받침대 위로 살짝 띄움
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 0.18f, 0.16f, 0.6f, 32, 32); // 물 높이
    glTranslatef(0, 0, 0.6f);
    gluDisk(quad, 0.0f, 0.16f, 32, 1);
    glTranslatef(0, 0, -0.6f);
    gluDisk(quad, 0.0f, 0.18f, 32, 1);

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
  
    glPopMatrix();

    glPopAttrib();
}

void UpdateWaterKettle()
{
    waterTime += 0.01f;

    // 선택된 상태라면 위로
    if (kettleSelected)
        kettleLift = std::min(0.3f, kettleLift + 0.02f);
    else
        kettleLift = std::max(0.0f, kettleLift - 0.02f);
}