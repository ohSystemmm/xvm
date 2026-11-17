#ifndef XVM_INPUT_DEVICE_HPP
#define XVM_INPUT_DEVICE_HPP

#include <cstdint>

namespace xvm {
    namespace buttons {
        constexpr uint8_t UP    = 1 << 0; // 0x01
        constexpr uint8_t DOWN  = 1 << 1; // 0x02
        constexpr uint8_t LEFT  = 1 << 2; // 0x04
        constexpr uint8_t RIGHT = 1 << 3; // 0x08
        constexpr uint8_t ACTION = 1 << 4; // 0x10
    }

    class InputDevice {
        public:
            InputDevice();
            ~InputDevice();

            void setButtonState(uint8_t buttonFlag, bool pressed);
            uint8_t readInput() const;

            void reset();

        private:
            uint8_t m_currentKeyState = 0;
        };
}

#endif