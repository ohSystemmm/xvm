#ifndef XVM_SYSTEM_CLOCK_HPP
#define XVM_SYSTEM_CLOCK_HPP

#include <chrono>

namespace xvm {
    class SystemClock {
    public:
        SystemClock();
        ~SystemClock();

        void start();
        void tick();
        double getDeltaTime() const;
        void limitFrameRate(int targetFPS);

    private:
        std::chrono::high_resolution_clock::time_point m_lastTime;
        double m_deltaTime;
        const double m_targetFrameTime = 1.0 / 60.0; // 60 FPS limite
    };
}

#endif