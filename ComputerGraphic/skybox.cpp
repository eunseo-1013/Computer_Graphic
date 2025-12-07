#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <glaux.h>

GLuint tex[6];   // Texture Mapping을 하기 위한 Texture 이미지의 개수를 위한 배열 변수

AUX_RGBImageRec *LoadBMP(const char *Filename) {  // Bitmap 이미지를 호출한다.
     FILE *File=NULL;

     if (!Filename) return NULL;
     File=fopen(Filename,"rb");
     if (File) {
          fclose(File);
          return auxDIBImageLoad(Filename);
     }

     return NULL;
}

void InitSkybox( ) {  // Bitmap 이미지 6개를 호출하여 Texture 이미지로 변환한다.
     AUX_RGBImageRec *texRec[6];
     memset(texRec, 0, sizeof(void *)*6);                                                  

     if((texRec[0]=LoadBMP("texture/posz.bmp")) &&
       (texRec[1]=LoadBMP("texture/negz.bmp")) &&
       (texRec[2]=LoadBMP("texture/posx.bmp")) &&
       (texRec[3]=LoadBMP("texture/negx.bmp")) &&
       (texRec[4]=LoadBMP("texture/posy.bmp")) &&
       (texRec[5]=LoadBMP("texture/negy.bmp")) ) {

          for(int i=0; i<6; i++) {
              printf("텍스처 로드: index %d (%d x %d)\n",
                  i, texRec[i]->sizeX, texRec[i]->sizeY);
               glGenTextures(1, &tex[i]); 
               glBindTexture(GL_TEXTURE_2D, tex[i]);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
               glTexImage2D(GL_TEXTURE_2D, 0, 3, texRec[i]->sizeX, texRec[i]->sizeY, 0,
                         GL_RGB, GL_UNSIGNED_BYTE, texRec[i]->data); 
          }
     }

     for(int i=0; i<6; i++) {                                                              
          if(texRec[i]) {
               if(texRec[i]->data) free(texRec[i]->data);
               free(texRec[i]);
          } 
     }

     glEnable(GL_TEXTURE_2D);
     glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void DrawSkybox( ) {
     float r = 50.0f;
     
     glDepthMask(GL_FALSE);

     glDisable(GL_LIGHTING);

     glColor3f(1.0f, 1.0f, 1.0f);

     // Skybox의 앞면을 Rendering 한다.
     glBindTexture(GL_TEXTURE_2D, tex[0]);
     glBegin(GL_QUADS);
          glTexCoord2f(0, 0); glVertex3f( -r, -r/2, -r );
          glTexCoord2f(1, 0); glVertex3f(  r, -r/2, -r );
          glTexCoord2f(1, 1); glVertex3f(  r, r, -r );
          glTexCoord2f(0, 1); glVertex3f( -r, r, -r );
     glEnd();

     // Skybox의 뒷면을 Rendering 한다.
     glBindTexture(GL_TEXTURE_2D, tex[1]);
     glBegin(GL_QUADS);
          glTexCoord2f(0, 0); glVertex3f(  r, -r/2,  r );
          glTexCoord2f(1, 0); glVertex3f( -r, -r/2,  r );
          glTexCoord2f(1, 1); glVertex3f( -r, r,  r );
          glTexCoord2f(0, 1); glVertex3f(  r, r,  r );
     glEnd();

     // Skybox의 오른쪽 면을 Rendering 한다.
     glBindTexture(GL_TEXTURE_2D, tex[2]);
     glBegin(GL_QUADS);
          glTexCoord2f(0, 0); glVertex3f(  r, -r/2, -r );
          glTexCoord2f(1, 0); glVertex3f(  r, -r/2,  r );
          glTexCoord2f(1, 1); glVertex3f(  r, r,  r );
          glTexCoord2f(0, 1); glVertex3f(  r, r, -r );
     glEnd();

     // Skybox의 왼쪽 면을 Rendering 한다.
     glBindTexture(GL_TEXTURE_2D, tex[3]);
     glBegin(GL_QUADS);
          glTexCoord2f(0, 0); glVertex3f( -r, -r/2,  r );
          glTexCoord2f(1, 0); glVertex3f( -r, -r/2, -r );
          glTexCoord2f(1, 1); glVertex3f( -r, r, -r );
          glTexCoord2f(0, 1); glVertex3f( -r, r,  r );
     glEnd();

     // Skybox의 윗면을 Rendering 한다.
     glBindTexture(GL_TEXTURE_2D, tex[4]);
     glBegin(GL_QUADS);
          glTexCoord2f(0, 0); glVertex3f( -r,  r, -r );
          glTexCoord2f(1, 0); glVertex3f(  r,  r, -r );
          glTexCoord2f(1, 1); glVertex3f(  r,  r,  r );
          glTexCoord2f(0, 1); glVertex3f( -r,  r,  r );
     glEnd();

     // Skybox의 아랫면을 Rendering 한다.
     glBindTexture(GL_TEXTURE_2D, tex[5]);
     glBegin(GL_QUADS);
          glTexCoord2f(0, 0); glVertex3f( -r, -r/2,  r );
          glTexCoord2f(1, 0); glVertex3f(  r, -r/2,  r );
          glTexCoord2f(1, 1); glVertex3f(  r, -r/2, -r );
          glTexCoord2f(0, 1); glVertex3f( -r, -r/2, -r );
     glEnd();

     // Skybox의 지표면을 Rendering 한다.
     /*glBindTexture(GL_TEXTURE_2D, tex[6]);
     glBegin(GL_QUADS);
          glTexCoord2f(0, 0); glVertex3f( -r, -r,  r );
          glTexCoord2f(1, 0); glVertex3f(  r, -r,  r );
          glTexCoord2f(1, 1); glVertex3f(  r, -r, -r );
          glTexCoord2f(0, 1); glVertex3f( -r, -r, -r );
     glEnd();*/

     glEnable(GL_LIGHTING);
     glDepthMask(GL_TRUE);
}