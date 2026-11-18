#include "devices/framebuffer.hpp"
#include "devices/io_ports.hpp"
#include "render/palette.hpp"

#include <algorithm>
#include <spdlog/spdlog.h>

namespace xvm {
    Framebuffer::Framebuffer(int width, int height)
        : m_width(width > 0 ? width : kDefaultWidth),
          m_height(height > 0 ? height : kDefaultHeight),
          m_pixels(static_cast<size_t>(m_width) * static_cast<size_t>(m_height), 0xFF000000) {}

    Framebuffer::~Framebuffer() = default;

    void Framebuffer::clear(uint32_t color) {
        std::fill(m_pixels.begin(), m_pixels.end(), color);
    }

    void Framebuffer::setPixel(int x, int y, uint32_t color) {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            m_pixels[static_cast<size_t>(y) * static_cast<size_t>(m_width) + static_cast<size_t>(x)] = color;
        }
    }

    uint32_t Framebuffer::getPixel(int x, int y) const {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            return m_pixels[static_cast<size_t>(y) * static_cast<size_t>(m_width) + static_cast<size_t>(x)];
        }
        return 0;
    }

    const uint32_t* Framebuffer::getBuffer() const {
        return m_pixels.data();
    }

    int Framebuffer::getWidth() const {
        return m_width;
    }

    int Framebuffer::getHeight() const {
        return m_height;
    }

    void Framebuffer::writePort(uint8_t port, uint8_t value) {
        switch (port) {
            case io_ports::FB_X:
                m_curX = value;
                break;
            case io_ports::FB_Y:
                m_curY = value;
                break;
            case io_ports::FB_DRAW:
                setPixel(m_curX, m_curY, paletteToRgba(value));
                break;
            case io_ports::FB_CLEAR:
                clear(paletteToRgba(value));
                break;
            default:
                spdlog::warn("Framebuffer::writePort: Unknown port 0x{:02X}", port);
                break;
        }
    }

    uint8_t Framebuffer::readPort(uint8_t port) const {
        switch (port) {
            case io_ports::FB_X:
                return m_curX;
            case io_ports::FB_Y:
                return m_curY;
            default:
                spdlog::warn("Framebuffer::readPort: Unknown port 0x{:02X}", port);
                return 0;
        }
    }

    void Framebuffer::reset() {
        m_curX = 0;
        m_curY = 0;
        clear(0xFF000000);
    }
}