#include "render/renderer.hpp"
#include "devices/framebuffer.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <algorithm>
#include <cmath>
#include <spdlog/spdlog.h>

namespace xvm {
    Renderer::Renderer() = default;

    Renderer::~Renderer() {
        shutdown();
    }

    bool Renderer::init() {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            spdlog::error("SDL_Init failed: {}", SDL_GetError());
            return false;
        }

        SDL_DisplayID primary = SDL_GetPrimaryDisplay();
        if (primary == 0) {
            spdlog::error("SDL_GetPrimaryDisplay failed: {}", SDL_GetError());
            SDL_Quit();
            return false;
        }

        // Fullscreen window
        // SDL_Rect usable{};
        // if (!SDL_GetDisplayUsableBounds(primary, &usable)) {
        //     spdlog::error("SDL_GetDisplayUsableBounds failed: {}", SDL_GetError());
        //     SDL_Quit();
        //     return false;
        // }

        // m_width  = usable.w;
        // m_height = usable.h;
        // spdlog::info("Primary display usable area: {}x{}", m_width, m_height);

        // m_window = SDL_CreateWindow("xvm", m_width, m_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);

        // Square window
        m_width  = 1024;
        m_height = 1024;
        
        m_window = SDL_CreateWindow("xvm", m_width, m_height, !SDL_WINDOW_RESIZABLE);  
        if (!m_window) {
            spdlog::error("SDL_CreateWindow failed: {}", SDL_GetError());
            SDL_Quit();
            return false;
        }

        // App icon
        SDL_Surface* iconSurface = IMG_Load("assets/icon.png");
        if (!iconSurface) {
            spdlog::warn("Failed to load icon image: {}", SDL_GetError());
        } else {
            if (!SDL_SetWindowIcon(m_window, iconSurface)) {
                spdlog::warn("Failed to set window icon: {}", SDL_GetError());
            }
            SDL_DestroySurface(iconSurface);
        }

        m_renderer = SDL_CreateRenderer(m_window, nullptr);
        if (!m_renderer) {
            spdlog::error("SDL_CreateRenderer failed: {}", SDL_GetError());
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
            SDL_Quit();
            return false;
        }

        spdlog::info("Renderer initialised ({}x{})", m_width, m_height);
        return true;
    }

    void Renderer::shutdown() {
        if (m_texture) {
            SDL_DestroyTexture(m_texture);
            m_texture = nullptr;
        }
        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        SDL_Quit();
        spdlog::info("Renderer shut down");
    }

    void Renderer::clear() {
        if (m_renderer) {

            SDL_SetRenderDrawColorFloat(m_renderer, 0, 0, 0, 255);
            SDL_RenderClear(m_renderer);
        }
    }

    void Renderer::present() {
        if (m_renderer) {
            SDL_RenderPresent(m_renderer);
        }
    }

    void Renderer::render(const Framebuffer& framebuffer) {
        if (!m_renderer) {
            return;
        }

        const int fbW = framebuffer.getWidth();
        const int fbH = framebuffer.getHeight();

        if (!m_texture || m_texWidth != fbW || m_texHeight != fbH) {
            if (m_texture) {
                SDL_DestroyTexture(m_texture);
                m_texture = nullptr;
            }
            m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, fbW, fbH);
            if (!m_texture) {
                spdlog::error("SDL_CreateTexture failed: {}", SDL_GetError());
                return;
            }
            SDL_SetTextureScaleMode(m_texture, SDL_SCALEMODE_NEAREST);
            m_texWidth = fbW;
            m_texHeight = fbH;
        }

        SDL_UpdateTexture(m_texture, nullptr, framebuffer.getBuffer(), fbW * sizeof(uint32_t));
        SDL_SetRenderDrawColor(m_renderer, 16, 16, 16, 255);
        SDL_RenderClear(m_renderer);

        int windowW = 0;
        int windowH = 0;
        SDL_GetWindowSize(m_window, &windowW, &windowH);

        float scale = std::floor(std::min(static_cast<float>(windowW) / fbW, static_cast<float>(windowH) / fbH));
        if (scale < 1.0f) {
            scale = 1.0f;
        }

        const float dstW = static_cast<float>(fbW) * scale;
        const float dstH = static_cast<float>(fbH) * scale;
        const float dstX = (static_cast<float>(windowW) - dstW) / 2.0f;
        const float dstY = (static_cast<float>(windowH) - dstH) / 2.0f;

        const SDL_FRect dstRect{dstX, dstY, dstW, dstH};
        SDL_RenderTexture(m_renderer, m_texture, nullptr, &dstRect);
        SDL_RenderPresent(m_renderer);
    }
}