#include "log/logger.hpp"
#include "render/renderer.hpp"
#include "vm/virtual_machine.hpp"
#include "devices/input_codes.hpp"

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>
#include <string>

int main(int argc, char* argv[]) {
    xvm::initLogger();
    spdlog::info("Starting XVM Virtual Machine...");

    xvm::VirtualMachine vm;

    std::string romPath;
    if (argc > 1) {
        romPath = argv[1];
    } else {
        romPath = "rom/bin/rom.bin";
        spdlog::info("No ROM specified, attempting to load default: {}", romPath);
    }

    if (!vm.loadRom(romPath)) {
        spdlog::error("Failed to load ROM: {}", romPath);
        if (argc <= 1) {
            spdlog::info("Usage: {} <path_to_rom.bin>", argv[0]);
        }
        return 1;
    }

    xvm::Renderer renderer;
    if (!renderer.init()) {
        return 1;
    }

    bool running = true;
    SDL_Event event{};
    auto& clock = vm.getClock();
    clock.start();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                const bool pressed = (event.type == SDL_EVENT_KEY_DOWN);
                switch (event.key.key) {
                    case SDLK_UP: {
                    }

                    case SDLK_W: {
                        vm.setButtonState(xvm::buttons::UP, pressed);
                        break;
                    }

                    case SDLK_DOWN: {
                    }

                    case SDLK_S: {
                        vm.setButtonState(xvm::buttons::DOWN, pressed);
                        break;
                    }

                    case SDLK_LEFT: {
                    }

                    case SDLK_A: {
                        vm.setButtonState(xvm::buttons::LEFT, pressed);
                        break;
                    }

                    case SDLK_RIGHT: {
                    }

                    case SDLK_D: {
                        vm.setButtonState(xvm::buttons::RIGHT, pressed);
                        break;
                    }

                    case SDLK_SPACE: {
                    }

                    case SDLK_RETURN: {
                        vm.setButtonState(xvm::buttons::ACTION, pressed);
                        break;
                    }

                    case SDLK_ESCAPE: {
                        running = false;
                        break;
                    }

                    default: {
                        break;
                    }
                }
            }
        }

        clock.tick();

        vm.runFrame(500);                       // (500 cpu instructions per frame)
        renderer.render(vm.getFramebuffer());   // Render virtual framebuffer
        clock.limitFrameRate(60);
    }

    return 0;
}