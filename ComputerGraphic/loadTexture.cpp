
#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include "glaux.h"

AUX_RGBImageRec* LoadBMPT(const char* filename)
{
    FILE* file = NULL;
    if (!filename) return NULL;

    printf("LoadBMP 시도 중: [%s]\n", filename);

    file = fopen(filename, "rb");
    if (!file) {
        printf("fopen 실패! 파일 없음: %s\n", filename);
        return NULL;
    }
    fclose(file);

    printf("auxDIBImageLoad 호출!\n");
    return auxDIBImageLoad(filename);
}


// glaux 방식 ==> gloadtexture



GLuint gLoadTexture(const char* filename)
{
    AUX_RGBImageRec* img = LoadBMPT(filename);
    if (!img) {
        printf("텍스처 파일을 읽을 수 없음: %s\n", filename);
        return false;
    }

    printf("텍스처 로드 성공: %s (%d x %d)\n",
        filename, (int)img->sizeX, (int)img->sizeY);

    GLuint TextureID;

    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        img->sizeX,
        img->sizeY,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        img->data
    );

    printf("%s 성공적 종료\n",filename);
    return TextureID;
}




