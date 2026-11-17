#ifndef XVM_FRAMEBUFFER_HPP
#define XVM_FRAMEBUFFER_HPP

#include <cstdint>
#include <vector>

namespace xvm {
    class Framebuffer {
        public:
            static constexpr int kDefaultWidth = 64;
            static constexpr int kDefaultHeight = 64;

            Framebuffer(int width = kDefaultWidth, int height = kDefaultHeight);
            ~Framebuffer();

            void clear(uint32_t color = 0xFF000000);
            void setPixel(int x, int y, uint32_t color);
            uint32_t getPixel(int x, int y) const;
            
            const uint32_t* getBuffer() const;
            int getWidth() const;
            int getHeight() const;

        private:
            int m_width;
            int m_height;
            std::vector<uint32_t> m_pixels;
        };
}

#endif