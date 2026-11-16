#include "log/logger.hpp"
#include "render/renderer.hpp"

#include <SDL3/SDL.h>
#include <iostream>

int main() {
    std::cout << "XVM" << std::endl;

    xvm::initLogger();

    xvm::Renderer renderer;
    if (!renderer.init()) {
        return 1;
    }

    bool running = true;
    SDL_Event event{};

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        renderer.clear();
        renderer.present();
        SDL_Delay(16);
    }

    return 0;
}