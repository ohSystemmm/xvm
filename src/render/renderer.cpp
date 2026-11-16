#include "render/renderer.hpp"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
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

        // fullscreen
        SDL_Rect usable{};
        if (!SDL_GetDisplayUsableBounds(primary, &usable)) {
            spdlog::error("SDL_GetDisplayUsableBounds failed: {}", SDL_GetError());
            SDL_Quit();
            return false;
        }

        m_width  = usable.w;
        m_height = usable.h;
        spdlog::info("Primary display usable area: {}x{}", m_width, m_height);

        // titlebar
        m_window = SDL_CreateWindow("xvm", m_width, m_height,SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
        if (!m_window) {
            spdlog::error("SDL_CreateWindow failed: {}", SDL_GetError());
            SDL_Quit();
            return false;
        }

        // appicon
        SDL_Surface* iconSurface = IMG_Load("assets/icon.png");
        if (!iconSurface) {
            spdlog::error("Failed to load icon image: {}", SDL_GetError());
        } else {
            if (!SDL_SetWindowIcon(m_window, iconSurface)) {
                spdlog::error("Failed to set window icon: {}", SDL_GetError());
            }
        }
        SDL_DestroySurface(iconSurface);

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
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_renderer);
    }

    void Renderer::present() {
        SDL_RenderPresent(m_renderer);
    }
}