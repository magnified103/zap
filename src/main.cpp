#include <iostream>
#include <iomanip>
#include <random>

#include "../include/wrapper/SDL.hpp"

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void log(bool fatal) {
    std::cerr << SDL_GetError() << std::endl;
    if (fatal) {
        exit(1);
    }
}

void reset(sdl::renderer &renderer) {
    if (renderer.set_draw_color({0x00, 0x00, 0x00}) < 0 ||
        renderer.clear() < 0) {
        log(true);
    }
}

void draw(sdl::renderer &renderer, int x, int y, int len) {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<> dist(0, 255);
    sdl::rect rect(x, y, len, len);
    if (renderer.set_draw_color({dist(rng), dist(rng), dist(rng)}) < 0 ||
        renderer.fill_rect(rect) < 0) {
        log(true);
    }
}

int main() {
    sdl::sdl sdl;
    sdl::window window("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    auto renderer = window.create_renderer(-1, SDL_RENDERER_ACCELERATED);
    reset(renderer);
    int x = 0, y = 0;
    draw(renderer, x, y, 40);
    renderer.present();
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        bool r = false, redraw = false;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYUP) {
                redraw = true;
                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    if (y >= 40) {
                        y -= 40;
                    }
                    break;
                case SDLK_DOWN:
                    if (y + 40 < SCREEN_HEIGHT) {
                        y += 40;
                    }
                    break;
                case SDLK_LEFT:
                    if (x >= 40) {
                        x -= 40;
                    }
                    break;
                case SDLK_RIGHT:
                    if (x + 40 < SCREEN_WIDTH) {
                        x += 40;
                    }
                    break;
                default:
                    r = true;
                    break;
                }
            } else if (e.type == SDL_WINDOWEVENT) {
                redraw = true;
            }
        }
        if (r) {
            x = y = 0;
            reset(renderer);
        }
        if (redraw) {
            draw(renderer, x, y, 40);
            renderer.present();
        }
        sdl::delay(16);
    }
}