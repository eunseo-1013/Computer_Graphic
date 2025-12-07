#pragma once
extern float g_value;   // 0.0 ~ 1.0 사이 게이지 값

void drawCircularGauge(float cx, float cy,
    float radius, float thickness,
    float value);