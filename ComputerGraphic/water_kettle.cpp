#include "water_kettle.h"
#include <iostream>

void DrawWaterKettle(float x, float y, float z) {
	glPushMatrix();
	glTranslatef(x, y, z);

	glScalef(0.7f, 0.7f, 0.7f);

    GLUquadric* quad = gluNewQuadric();

    // 1. ¹Ø¹ÞÄ§
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 0.40f, 0.40f, 0.05f, 32, 32); // ¹ÞÄ§´ë
    gluDisk(quad, 0.0f, 0.40f, 32, 1);             // ¹Ù´Ú
    glTranslatef(0.0f, 0.0f, 0.05f);
    gluDisk(quad, 0.0f, 0.40f, 32, 1);             // À­¸é
    glPopMatrix();

    // 2. ¹°
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glColor4f(0.4f, 0.6f, 1.0f, 0.5f);
    glPushMatrix();
    glTranslatef(0.0f, 0.06f, 0.0f); // ¹ÞÄ§´ë À§·Î »ìÂ¦ ¶ç¿ò
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 0.35f, 0.35f, 0.6f, 32, 32); // ¹° ³ôÀÌ
    glPopMatrix();

    // 3. À¯¸® ¸öÅë
    glColor4f(0.9f, 0.95f, 1.0f, 0.2f);
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.0f); 
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 0.38f, 0.35f, 0.7f, 32, 32);
    glPopMatrix();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

    // 4. ¶Ñ²±
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.0f, 0.75f, 0.0f); // ¸öÅë À§
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    gluCylinder(quad, 0.35f, 0.30f, 0.08f, 32, 32);
    glTranslatef(0.0f, 0.0f, 0.08f);
    gluDisk(quad, 0.0f, 0.30f, 32, 1); // ¶Ñ²± ´Ý±â
    glPopMatrix();

    // 5. ÁÖµÕÀÌ
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(-0.3f, 0.78f, 0.0f);  // ¶Ñ²± ¿ÞÂÊ
    glRotatef(-90, 0, 1, 0);
    
    gluCylinder(quad, 0.04f, 0.01f, 0.10f, 16, 16);  // ³¡ (»ÏÁ·)

    glPopMatrix();

    // 6. ¼ÕÀâÀÌ
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0.45f, 0.45f, 0.0f);   // Æ÷Æ® ¿À¸¥ÂÊ¿¡ ºÙÀÌ±â

    // ¼¼·Î ¸·´ë
    glPushMatrix();
    glTranslatef(0.1f, 0.0f, 0.0f);
    glScalef(0.1f, 0.4f, 0.1f);      // Æø, ³ôÀÌ, µÎ²²
    glutSolidCube(1.0f);
    glPopMatrix();

    // ¾Æ·¡ °¡·Î ¸·´ë
    glPushMatrix();
    glTranslatef(0.485f, 0.2f, 0.0f);
    glScalef(0.23f, 0.1f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // À§ °¡·Î ¸·´ë
    glPushMatrix();
    glTranslatef(0.475f, 0.7f, 0.0f);
    glScalef(0.25f, 0.1f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ¿¬°áºÎ
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();

    glTranslatef(0.36f, 0.4f, 0.0f);
    glRotatef(2.5f, 0, 0, 1);

    glScalef(0.01f, 0.7f, 0.1f);
    glutSolidCube(1.0f);

    glPopMatrix();

    glPopMatrix();
}