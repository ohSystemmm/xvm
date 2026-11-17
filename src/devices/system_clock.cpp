#include "devices/system_clock.hpp"

#include <thread>

namespace xvm {
    SystemClock::SystemClock()
        : m_lastTime(std::chrono::high_resolution_clock::now()), m_deltaTime(0.0) {}

    SystemClock::~SystemClock() = default;

    void SystemClock::start() {
        m_lastTime = std::chrono::high_resolution_clock::now();
        m_deltaTime = 0.0;
    }

    void SystemClock::tick() {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - m_lastTime;
        m_deltaTime = elapsed.count();
        m_lastTime = now;
    }

    double SystemClock::getDeltaTime() const {
        return m_deltaTime;
    }

    void SystemClock::limitFrameRate(int targetFPS) {
        if (targetFPS <= 0) return;
        const double targetDuration = 1.0 / static_cast<double>(targetFPS);
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = now - m_lastTime;
        if (elapsed.count() < targetDuration) {
            double sleepSeconds = targetDuration - elapsed.count();
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepSeconds));
        }
    }
}