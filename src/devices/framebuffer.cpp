#include "devices/framebuffer.hpp"

#include <algorithm>

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
}