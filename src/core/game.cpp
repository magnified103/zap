#include <fstream>
#include <memory>

#include "../../include/core/game.hpp"
#include "../../include/geometry/point.hpp"
#include "../../include/wrapper/SDL.hpp"

void game::initialize() {

    // load map
    main_map = std::make_unique<map_grid>(std::ifstream("map.txt"));

    // specify window sizes
    window_width = 800;
    window_height = 640;

    // create window
    main_window =
        std::make_unique<sdl::window>("zap", window_width, window_height);

    // create renderer
    main_renderer = std::make_unique<sdl::renderer>(
        main_window
            ->create_renderer(SDL_RENDERER_ACCELERATED |
                              SDL_RENDERER_PRESENTVSYNC)
            .get());

    // initial position
    int grid_initial_x, grid_initial_y;
    std::tie(grid_initial_x, grid_initial_y) =
        main_map->get_starting_position();

    // temporary values
    point2d<float> player_position = {static_cast<float>(grid_initial_x),
                                      static_cast<float>(grid_initial_y)};

    {
        float grid_size = main_map->get_grid_size();
        player_position = player_position * grid_size +
                          point2d<float>{grid_size / 2, grid_size / 2};
    }

    // create player
    main_player = std::make_unique<player>(player_position, 0, 0.1, 0.003);

    // keyboard state
    keyboard_state = SDL_GetKeyboardState(nullptr);

    // camera
    main_camera = std::make_unique<camera>();

    // timer start
    game_timer.start();
}

void game::game_logic() {
    Uint32 diff_time = game_timer.get_ticks();
    game_timer.start();
    int move_type = movement_type::none;
    if (keyboard_state[SDL_SCANCODE_W]) {
        move_type = movement_type::up;
    } else if (keyboard_state[SDL_SCANCODE_S]) {
        move_type = movement_type::down;
    }
    float c = 0;
    if (keyboard_state[SDL_SCANCODE_A]) {
        c = -1;
    } else if (keyboard_state[SDL_SCANCODE_D]) {
        c = 1;
    }
    auto player_position = main_player->try_move(move_type, diff_time);
    if (!main_map->collide(std::floor(player_position.x),
                           std::floor(player_position.y))) {
        main_player->set_position(player_position);
    }
    main_player->rotate_camera(c, diff_time);
    main_renderer->clear_with(sdl::color{0x00, 0x00, 0x00});
    main_camera->render(*main_renderer, *main_player, *main_map, window_width,
                        window_height);
    main_renderer->present();
}