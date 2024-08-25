#include "timer.hpp"
#include "constants.hpp"

Timer::Timer() : m_time(TOTAL_TIME), m_frameCount(0) {}

void Timer::Update() {
    m_frameCount++;
    if (m_frameCount >= 60) {
        m_frameCount = 0;
        if (m_time > 0) {
            m_time--;
        }
    }
}

void Timer::Draw() const {
    DrawText(TextFormat("Time: %02i", m_time), 600, 10, 20, BLACK);
}