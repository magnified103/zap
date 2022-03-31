#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <utility>

#include "../include/geometry/point.hpp"
#include "../include/scenery/camera.hpp"
#include "../include/scenery/dot.hpp"
#include "../include/scenery/map.hpp"
#include "../include/various/timer.hpp"
#include "../include/wrapper/SDL.hpp"

int main(int argc, char *argv[]) {
    sdl::sdl sdl;
    map_grid grid(std::ifstream("map.txt"));

    // get window sizes
    // int window_width = grid.get_width() * grid.get_grid_size();
    // int window_height = grid.get_height() * grid.get_grid_size();

    int window_width = 800;
    int window_height = 640;

    // create window
    sdl::window window("zap", window_width, window_height);

    // create renderer
    sdl::renderer renderer = window.create_renderer(SDL_RENDERER_ACCELERATED |
                                                    SDL_RENDERER_PRESENTVSYNC);

    // background texture
    sdl::texture background_texture =
        renderer.create_texture(grid.get_surface());

    // dot surface
    sdl::surface dot_surface(10, 10);
    dot_surface.fill(sdl::color{0x00, 0xff, 0xff});

    // dot texture
    sdl::texture dot_texture = renderer.create_texture(dot_surface);

    // get background sizes
    int background_width = background_texture.get_width();
    int background_height = background_texture.get_height();

    // event dispatcher
    sdl::event_dispatcher<SDL_QuitEvent> dispatcher;

    // initial position
    int grid_initial_x, grid_initial_y;
    std::tie(grid_initial_x, grid_initial_y) = grid.get_starting_position();

    // temporary values
    point2d<float> player_position = {grid_initial_x, grid_initial_y};

    {
        float grid_size = grid.get_grid_size();
        player_position = player_position * grid_size +
                          point2d<float>{grid_size / 2, grid_size / 2};
    }

    // dot
    dot player{player_position, 0, 0.1, 0.003};

    // quit flag
    bool quit_flag = false;

    dispatcher.bind<SDL_QuitEvent>().to(
        [&](const SDL_QuitEvent &event) { quit_flag = true; });

    timer game_timer;
    game_timer.start();

    const Uint8 *keyboard_state = SDL_GetKeyboardState(nullptr);

    camera game_camera;

    while (!quit_flag) {
        dispatcher.poll();

        // renderer.render(background_texture,
        //                 sdl::rect{0, 0, background_width,
        //                 background_height});
        Uint32 diff_time = game_timer.get_ticks();
        game_timer.start();
        int move_type = movement_type::none;
        if (keyboard_state[SDL_SCANCODE_W]) {
            move_type = movement_type::up;
        } else if (keyboard_state[SDL_SCANCODE_S]) {
            move_type = movement_type::down;
        }
        float c = 0;
        if (keyboard_state[SDL_SCANCODE_LEFT]) {
            c = -1;
        } else if (keyboard_state[SDL_SCANCODE_RIGHT]) {
            c = 1;
        }
        player_position = player.try_move(move_type, diff_time);
        if (!grid.collide(std::floor(player_position.x),
                          std::floor(player_position.y))) {
            player.set_position(player_position);
        }
        player.rotate_camera(c, diff_time);
        // player.render(renderer, dot_texture);
        sdl::surface fov_surface(window_height, window_width);
        fov_surface.fill(sdl::color{0x00, 0x00, 0x00});
        game_camera.render(fov_surface, player, grid, window_width, window_height);
        sdl::texture fov_texture = renderer.create_texture(fov_surface);
        renderer.render(fov_texture);
        renderer.present();
    }
}