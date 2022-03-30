#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

#include "../include/scenery/map.hpp"
#include "../include/wrapper/SDL.hpp"

int main(int argc, char *argv[]) {
    sdl::sdl sdl;
    map_grid grid(std::cin);
    int window_width = grid.get_width() * grid.get_grid_size();
    int window_height = grid.get_height() * grid.get_grid_size();
    sdl::window window{
        "zap",        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_width, window_height,           SDL_WINDOW_SHOWN};
    sdl::renderer renderer{window.create_renderer(SDL_RENDERER_ACCELERATED)};
    sdl::surface background_surface{grid.get_surface()};
    sdl::texture background{renderer.create_texture(background_surface)};
    renderer.render(background, sdl::rect{0, 0, background.get_width(),
                                          background.get_height()});
    renderer.present();
    sdl::delay(5000);
}