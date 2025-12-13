#pragma once

extern GLuint gNoiseTex0;
extern GLuint gNoiseTex1;
extern GLuint gNoiseTex2;


void SetupFog();

void DrawNoiseOverlay(float alpha,int num);


bool InitTextures();