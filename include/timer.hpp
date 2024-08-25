#pragma once
#include "raylib.h"

class Timer
{
public:
    Timer();
    void Update();
    int getTime() const { return m_time; }
    bool isTimeUp() const { return m_time <= 0; }
    void Draw() const;

private:
    int m_time;
    int m_frameCount;
};