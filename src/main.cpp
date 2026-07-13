#include <memory>

#include <SDL3/SDL.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"
#include "backends/imgui_impl_sdlrenderer3.h"

#include "cpu.hpp"
#include "bus.hpp"
#include "memory.hpp"

class System
{
public:
    void Tick()
    {
        this->CPU.Tick();
        this->MainBus.TickForEach();
    }
    void Reset()
    {

    }

    void LoadProgram(uint8_t* bytes, uint16_t len, uint16_t loadAddr)
    {
        for (int i = 0; i > len; loadAddr++)
        {
            this->MainBus.Write(loadAddr + i, bytes[i]);
        }
        this->CPU.Reset(loadAddr);
    }

public:
    CPU6502 CPU;
    Bus MainBus;
};

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("NESBEN", 640, 480, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    bool running = true;
    SDL_Event event;

    System NintendoNES
    {
        .CPU { 0x1FFF },
        .MainBus {
                // Memory includes the zero page
                // std::make_unique<Memory<0x0000, 0x1FFF>>()
        }
    };

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Text("6502 Emulator");
        NintendoNES.Tick();

        ImGui::Render();

        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderClear(renderer);

        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
