#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_sdlrenderer2.h>

#include <stdio.h>
#include <string>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#include "texture.cpp"
#include "timer.cpp"

const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 480;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Texture colored_sheet;
Texture transparent_sheet;

int init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Cannot initialize SDL! %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow(
            "SDL",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        fprintf(stderr, "Window cannot be created! %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "Renderer cannot be created! %s\n", SDL_GetError());
        return -1;
    }

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_SetHint ("SDL_RENDER_BATCHING", "1") ;

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "SDL_image cannot be initialized! %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "SDL_TTF cannot be initialized! %s\n", SDL_GetError());
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    return 0;
}

bool load_res() {
    if (!colored_sheet.load_from_file(renderer, "res/colored_packed_rgb.png")) return false;
    if (!transparent_sheet.load_from_file(renderer, "res/transparent_packed_rgb.png")) return false;
    transparent_sheet.set_blend_mode(SDL_BLENDMODE_BLEND);
    return true;
}

void destroy() {
    transparent_sheet.free();
    colored_sheet.free();

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void render_sheet_frame(Texture sheet, int x, int y, int tileX, int tileY) {
    SDL_Rect frame = { tileX*16, tileY*16, 16, 16 };
    sheet.render(x*16, y*16, &frame);
}

int main() {
    if (init() != 0) return -1;
    if (!load_res()) return -1;

    SDL_Event e;
    bool running = true;

    Timer fps_timer;
    int fps = 0;
    int counted_frames = 0;
    fps_timer.start();

    while (running) {
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT) running = false;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                }
            }
        }

        SDL_RenderClear(renderer);
        if (fps_timer.get_ticks() > 1000) {
            fps = counted_frames;
            counted_frames = 0;
            fps_timer.start();
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::Text("FPS: %d", fps);

        for (int i = 0; i < 100000; i++) {
            render_sheet_frame(colored_sheet, 0, 0, 0, 0);
            render_sheet_frame(transparent_sheet, 0, 0, 0, 1);
        }

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
        counted_frames++;
    }

    return 0;
}
