#ifndef XVM_INPUT_DEVICE_HPP
#define XVM_INPUT_DEVICE_HPP

#include <cstdint>

namespace xvm {
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