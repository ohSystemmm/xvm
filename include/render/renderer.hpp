#ifndef XVM_RENDERER_HPP
#define XVM_RENDERER_HPP

#include <SDL3/SDL.h>

namespace xvm {
    class Renderer {
        public:
            Renderer();
            ~Renderer();

            Renderer(const Renderer&)            = delete;
            Renderer& operator=(const Renderer&) = delete;

            bool init();
            void shutdown();
            void clear();
            void present();

            [[nodiscard]] SDL_Window*   getWindow()   const { return m_window; }
            [[nodiscard]] SDL_Renderer* getSDLRenderer() const { return m_renderer; }
            [[nodiscard]] int           getWidth()    const { return m_width; }
            [[nodiscard]] int           getHeight()   const { return m_height; }

        private:
            SDL_Window*   m_window   = nullptr;
            SDL_Renderer* m_renderer = nullptr;
            int           m_width    = 0;
            int           m_height   = 0;
    };
}

#endif