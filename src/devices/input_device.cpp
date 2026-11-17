#include "devices/input_device.hpp"

namespace xvm {
    InputDevice::InputDevice() = default;
    InputDevice::~InputDevice() = default;

    void InputDevice::setButtonState(uint8_t buttonFlag, bool pressed) {
        if (pressed) {
            m_currentKeyState |= buttonFlag;
        } else {
            m_currentKeyState &= static_cast<uint8_t>(~buttonFlag);
        }
    }

    uint8_t InputDevice::readInput() const {
        return m_currentKeyState;
    }

    void InputDevice::reset() {
        m_currentKeyState = 0;
    }
}