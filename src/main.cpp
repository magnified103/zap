#include <iomanip>
#include <iostream>
#include <random>

#include "../include/wrapper/SDL.hpp"

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void log(bool fatal) {
    sdl::log_error("%s", SDL_GetError());
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
    Uint8 r = dist(rng);
    Uint8 g = dist(rng);
    Uint8 b = dist(rng);
    if (renderer.set_draw_color({r, g, b}) < 0 ||
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
    bool quit = false, redraw_flag = false, reset_flag = false;
    sdl::event_dispatcher<SDL_QuitEvent, SDL_KeyboardEvent, SDL_WindowEvent>
        dispatcher;
    dispatcher.bind<SDL_QuitEvent>().to([&](const SDL_QuitEvent &e) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
    });
    dispatcher.bind<SDL_KeyboardEvent>().to([&](const SDL_KeyboardEvent &e) {
        if (e.type == SDL_KEYUP) {
            redraw_flag = true;
            switch (e.keysym.sym) {
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
                reset_flag = true;
                break;
            }
        }
    });
    dispatcher.bind<SDL_WindowEvent>().to(
        [&](const SDL_WindowEvent &e) { redraw_flag = true; });
    while (!quit) {
        reset_flag = false, redraw_flag = false;
        dispatcher.poll();
        if (reset_flag) {
            x = y = 0;
            reset(renderer);
        }
        if (redraw_flag) {
            draw(renderer, x, y, 40);
            renderer.present();
        }
        sdl::delay(16);
    }
}