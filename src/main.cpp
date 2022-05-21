#include <fstream>
#include <memory>
#include <string>

#include "cli/CLI11.hpp"
#include "core/game.hpp"
#include "core/map_editor.hpp"
#include "opengl/loader.hpp"

int main(int argc, char *argv[]) {
    CLI::App app;

    bool edit_mode{false};
    app.add_flag("--edit,-e", edit_mode, "Activate versatile edit mode");

    std::string map_path{"map.json"};
    app.add_option("--map,-m", map_path, "Specify the map to be loaded");

    CLI11_PARSE(app, argc, argv);

    if (edit_mode) {
        map_editor(map_path);
        return 0;
    }

    sdl::sdl sdl;
    sdl::img img;
    gles_loader loader;

    // declare game
    std::unique_ptr<basic_game> zap = std::make_unique<game>(map_path);

    // run game
    return zap->run();

    // map_grid grid(std::ifstream("map.txt"));

    // // get window sizes
    // int minimap_width = grid.get_width() * grid.get_grid_size();
    // int minimap_height = grid.get_height() * grid.get_grid_size();

    // int window_width = 800;
    // int window_height = 640;

    // // create window
    // sdl::window window("zap", window_width, window_height);

    // // minimap window
    // sdl::window minimap_window("minimap", minimap_width, minimap_height);

    // // create renderer
    // sdl::renderer renderer = window.create_renderer(SDL_RENDERER_ACCELERATED
    // |
    //                                                 SDL_RENDERER_PRESENTVSYNC).get();

    // // renderer for minimap
    // sdl::renderer minimap_renderer = minimap_window.create_renderer(
    //     SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC).get();

    // // minimap texture
    // sdl::texture background_texture =
    //     minimap_renderer.create_texture(grid.get_surface());

    // // player surface
    // sdl::surface dot_surface(10, 10);
    // dot_surface.fill(sdl::color{0x00, 0xff, 0xff});

    // // player texture
    // sdl::texture dot_texture = minimap_renderer.create_texture(dot_surface);

    // // get background sizes
    // int background_width = background_texture.get_width();
    // int background_height = background_texture.get_height();

    // // event dispatcher
    // sdl::event_dispatcher<SDL_WindowEvent> dispatcher;

    // // initial position
    // int grid_initial_x, grid_initial_y;
    // std::tie(grid_initial_x, grid_initial_y) = grid.get_starting_position();

    // // temporary values
    // point2d<float> player_position = {static_cast<float>(grid_initial_x),
    //                                   static_cast<float>(grid_initial_y)};

    // {
    //     float grid_size = grid.get_grid_size();
    //     player_position = player_position * grid_size +
    //                       point2d<float>{grid_size / 2, grid_size / 2};
    // }

    // // player
    // player player{player_position, 0, 0.1, 0.003};

    // // quit flag
    // bool quit_flag = false;

    // dispatcher.bind<SDL_WindowEvent>().to([&](const SDL_WindowEvent &event) {
    //     if (event.event == SDL_WINDOWEVENT_CLOSE) {
    //         quit_flag = true;
    //     }
    // });

    // timer game_timer;
    // game_timer.start();

    // const Uint8 *keyboard_state = SDL_GetKeyboardState(nullptr);

    // camera game_camera;

    // while (!quit_flag) {
    //     dispatcher.poll();

    //     minimap_renderer.render(
    //         background_texture,
    //         sdl::rect{0, 0, background_width, background_height});
    //     Uint32 diff_time = game_timer.get_ticks();
    //     game_timer.start();
    //     int move_type = movement_type::none;
    //     if (keyboard_state[SDL_SCANCODE_W]) {
    //         move_type = movement_type::up;
    //     } else if (keyboard_state[SDL_SCANCODE_S]) {
    //         move_type = movement_type::down;
    //     }
    //     float c = 0;
    //     if (keyboard_state[SDL_SCANCODE_A]) {
    //         c = -1;
    //     } else if (keyboard_state[SDL_SCANCODE_D]) {
    //         c = 1;
    //     }
    //     player_position = player.try_move(move_type, diff_time);
    //     if (!grid.collide(std::floor(player_position.x),
    //                       std::floor(player_position.y))) {
    //         player.set_position(player_position);
    //     }
    //     player.rotate_camera(c, diff_time);
    //     player.render(minimap_renderer, dot_texture);
    //     renderer.clear_with(sdl::color{0x00, 0x00, 0x00});
    //     game_camera.render(renderer, player, grid, window_width,
    //     window_height); renderer.present(); minimap_renderer.present();
    // }
}