#ifndef XVM_FRAMEBUFFER_HPP
#define XVM_FRAMEBUFFER_HPP

#include <cstdint>
#include <vector>

namespace xvm {
    class Framebuffer {
    public:
        Framebuffer(int width, int height);
        ~Framebuffer();

        void clear(uint32_t color);
        void setPixel(int x, int y, uint32_t color);
        uint32_t getPixel(int x, int y) const;
        const uint32_t* getBuffer() const;

        int getWidth() const;
        int getHeight() const;

    private:
        std::vector<uint32_t> m_pixels;
        int m_width;
        int m_height;
    };
}

#endif