#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

#include "core/game.hpp"
// #include "include/geometry/point.hpp"
#include "entity/weapon.hpp"
#include "geometry/matrix.hpp"
#include "geometry/trigonometric.hpp"
#include "geometry/vector.hpp"
#include "opengl/context.hpp"
#include "opengl/gl.hpp"
#include "opengl/utilities.hpp"
#include "scenery/map_util.hpp"
#include "sdl2/SDL.hpp"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                 GLsizei length, const GLchar *message, const void *userParam) {
    if (type == GL_DEBUG_TYPE_ERROR) {
        throw std::runtime_error(message);
    } else {
        sdl::log_info("Callback type=0x%x, severity=0x%x, message=%s", type, severity, message);
    }
}

game::game(const std::string &map_path) {

    // specify window sizes
    window_width = 800;
    window_height = 640;

    // create window
    main_window = std::make_unique<sdl::window>(
        "zap", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    context_loader context(main_window->get());

    // load map
    main_map = std::make_unique<map3d>();
    main_player = std::make_unique<player3d>();
    physics_engine = std::make_unique<physics>();
    load_map_and_textures(map_path, *main_map, *main_player, *physics_engine, surface_ids);

    // main_map->number_of_columns = 2;
    // main_map->number_of_rows = 2;
    // main_map->cell_size = 5.0f;
    // main_map->terrain = {{{-1, -1, 0, {0, 0, 0}, {5, 20, 5}, 5, 20, 1},
    //                       {-1, -1, 0, {0, 0, 5}, {5, 20, 10}, 5, 20, 1}},
    //                      {{-1, -1, 0, {5, 0, 0}, {10, 20, 5}, 5, 20, 1},
    //                       {-1, -1, 0, {5, 0, 5}, {10, 20, 10}, 5, 20, 1}}};
    // main_map->blocks = {{-1, -1, -1, -1, 0, -1, 10, 0}};
    // main_map->monsters.push_back({});
    // auto &monster = main_map->monsters[0];
    // monster.walk_speed = 0.5;
    // monster.run_speed = 0;
    // monster.jump_speed = 10;
    // monster.current_movement = {0, {5, 5, 5}};
    // // monster.current_inventory.items.push_back(std::make_unique<ranged_weapon>(0, false, 100,
    // // 100));
    // monster.current_inventory.selection = 0;
    // monster.hitbox_height = 5.6;
    // monster.hitbox_radius = 2.05;
    // monster.hitpoint = 100;
    // monster.front_id = 2;
    // main_map->projectile_types = {{10, 1, 0.1, 1, 0.1, 5, 5, 20}};
    // main_map->textures = {{0,
    //                        {{5, 5, 0}, {5, 5, 5}, {0, 5, 0}, {5, 5, 5}, {0, 5, 0}, {0, 5, 5}},
    //                        {{0, 0}, {0, 1}, {1, 0}, {0, 1}, {1, 0}, {1, 1}}}};
    // main_map->surfaces = {"./res/d2_flat1.png", "./res/plain-white.png", "./res/playa1.png"};
    // main_player->walk_speed = 0.5;
    // main_player->run_speed = 0;
    // main_player->jump_speed = 10;
    // main_player->current_movement = {0, {5, 5, 5}};
    // main_player->current_inventory.items.push_back(
    //     std::make_unique<ranged_weapon>(0, false, 100, 100));
    // main_player->current_inventory.selection = 0;
    // main_player->hitbox_height = 5;
    // main_player->hitbox_radius = 0.5;
    // main_player->hitpoint = 100;
    // main_player->camera_position = {0, 5, 0};
    // main_player->hand_position = {0, 5, 0};
    // main_player->camera = camera3d{radians(60.0f), 0.5f, 1000.0f, 0.0f, radians(90.0f)};
    // physics_engine->ai = std::make_unique<monster_ai_pathfinding>();
    // save_map("map.json", *main_map, *main_player, *physics_engine);

    // load_textures("map.json", *main_map, surface_ids);

    // vsync
    // if (sdl::gl_set_swap_interval(1) < 0) {
    //     throw std::runtime_error("Unable to turn VSync on");
    // }

    // debug trap
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    // w-component depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // // create renderer
    // main_renderer = std::make_unique<sdl::renderer>(
    //     main_window
    //         ->create_renderer(SDL_RENDERER_ACCELERATED |
    //                           SDL_RENDERER_PRESENTVSYNC)
    //         .get());

    // // initial position
    // int grid_initial_x, grid_initial_y;
    // std::tie(grid_initial_x, grid_initial_y) =
    //     main_map->get_starting_position();

    // // temporary values
    // point2d<float> player_position = {static_cast<float>(grid_initial_x),
    //                                   static_cast<float>(grid_initial_y)};

    // {
    //     float grid_size = main_map->get_grid_size();
    //     player_position = player_position * grid_size +
    //                       point2d<float>{grid_size / 2, grid_size / 2};
    // }

    // // create player
    // main_player = std::make_unique<player>(player_position, 0.1);

    // keyboard state
    keyboard_state = SDL_GetKeyboardState(nullptr);

    mouse_is_trapped = false;

    // // trap cursor
    // sdl::set_relative_mouse_mode(true);
    // mouse_is_trapped = true;

    shader_program = load_shader_program(R"(#version 400 core

    // Input vertex data, different for all executions of this shader.
    layout(location = 0) in vec3 vertexPosition_modelspace;
    layout(location = 1) in vec2 vertexUV;

    // Output data ; will be interpolated for each fragment.
    out vec2 UV;

    // Values that stay constant for the whole mesh.
    uniform mat4 MVP;

    void main() {
        // Output position of the vertex, in clip space : MVP * position
        gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

        // UV of the vertex. No special space for this one.
        UV = vertexUV;
    })",
                                         R"(#version 400 core

    // Interpolated values from the vertex shaders
    in vec2 UV;

    // Ouput data
    out vec4 color;

    // Values that stay constant for the whole mesh.
    uniform sampler2D texture_sampler;

    void main() {
        // Output color = color of the texture at the specified UV
        color = texture( texture_sampler, UV ).rgba;
    })");

    // timer start
    game_timer.start();

    // default clear color
    glClearColor(0, 0, 0, 1);
}

void game::game_logic() {
    Uint32 diff_time = game_timer.get_ticks();
    game_timer.start();

    // main_window->set_title("zap - fps=" + std::to_string(1000.0f /
    // diff_time));

    // int move_type = movement_type::none;
    float forward = 0, backward = 0, left = 0, right = 0;
    if (mouse_is_trapped || true) {
        if (keyboard_state[SDL_SCANCODE_W]) {
            // move_type = movement_type::up;
            forward = 1;
        }
        if (keyboard_state[SDL_SCANCODE_S]) {
            // move_type = movement_type::down;
            backward = 1;
        }
    }
    // float c = 0;
    bool jump = false;
    if (mouse_is_trapped || true) {
        if (keyboard_state[SDL_SCANCODE_A]) {
            left = 1;
        }
        if (keyboard_state[SDL_SCANCODE_D]) {
            right = 1;
        }
        if (keyboard_state[SDL_SCANCODE_SPACE]) {
            jump = true;
        }
    }
    physics_engine->trigger_player_keyboard(*main_map, *main_player, forward, backward, left, right,
                                            jump, false);
    // int type = 0;
    // if ((forward || backward || left || right)) {
    //     type = 1;
    // }
    // bool floating = !main_map->collide_floor(main_player->try_move(0, 0, 0, 0, 0, 0.1).position);
    // auto movement = main_player->try_move(forward, backward, left, right, type, delta_time);
    // if (main_map->collide_floor(movement.position)) {
    //     movement =
    //         main_player->try_move_without_gravity(forward, backward, left, right, type,
    //         delta_time);
    // }
    // if (main_map->collide_floor(movement.position)) {
    //     movement = main_player->try_move(0, 0, 0, 0, 0, delta_time);
    // }
    // if (!main_map->collide_floor(movement.position)) {
    //     main_player->current_movement = movement;
    // } else if (!floating) {
    //     // stair stepping?
    //     if (!main_map->collide_floor(movement.position, 0.2f)) {
    //         main_player->current_movement = movement;
    //     }
    // }
    // if (jump && !floating) {
    //     main_player->trigger_jump();
    // }

    // // handle projectiles
    // std::vector<projectile> trans_projectiles;
    // for (auto projectile : main_map->projectiles) {
    //     if (projectile.phase >= 2) {
    //         continue;
    //     }
    //     bool ended = false;
    //     bool newly_exploded = false;
    //     if (projectile.phase == 0) {
    //         auto next_position = projectile.try_move(delta_time);
    //         if (main_map->collide_floor(next_position) || main_map->collide_ceil(next_position))
    //         {
    //             sdl::log_info("Projectile collides with floor / ceil");
    //             newly_exploded = true;
    //             ended = true;
    //         } else {
    //             for (const auto &monster : main_map->monsters) {
    //                 if (monster.collide(next_position)) {
    //                     sdl::log_info("Projectile collides with monster");
    //                     newly_exploded = true;
    //                     ended = true;
    //                     break;
    //                 }
    //             }
    //         }
    //         projectile.position = projectile.try_move(delta_time);
    //     } else {
    //         ended = true;
    //     }
    //     if (ended) {
    //         projectile.explode(delta_time);
    //     }
    //     if (newly_exploded) {
    //         for (auto &monster : main_map->monsters) {
    //             if (monster.calculate_distance(projectile.position) <=
    //                 projectile.base.blast_radius) {
    //                 monster.hitpoint -= projectile.base.damage;
    //                 sdl::log_info("Projectile dealt %d, distance = %.2f", projectile.base.damage,
    //                               monster.calculate_distance(projectile.position));
    //             }
    //         }
    //     }
    //     trans_projectiles.push_back(projectile);
    // }
    // main_map->projectiles = trans_projectiles;

    // // handle monster deaths
    // std::vector<monster> trans_monsters;
    // for (const auto &monster : main_map->monsters) {
    //     if (monster.hitpoint > 0) {
    //         trans_monsters.push_back(monster);
    //     }
    // }
    // main_map->monsters = trans_monsters;

    // // handle monster physics
    // for (auto &monster : main_map->monsters) {
    //     auto movement = monster.try_move(vec2{}, 0, delta_time);
    //     if (!main_map->collide_floor(movement.position)) {
    //         monster.current_movement = movement;
    //     }
    // }

    static Uint32 lag_time = 0.0f;
    lag_time += diff_time;

    constexpr Uint32 MS_PER_UPDATE = 20;
    constexpr float DELTA_TIME = MS_PER_UPDATE * 0.001;

    while (lag_time >= MS_PER_UPDATE) {
        physics_engine->resolve(*main_map, *main_player, DELTA_TIME);
        main_player->current_inventory.update_state(DELTA_TIME);

        for (auto &monster : main_map->monsters) {
            monster.current_inventory.update_state(DELTA_TIME);
        }
        lag_time -= MS_PER_UPDATE;
    }

    // auto player_position =
    //     main_player->try_move(*main_camera, move_type, diff_time);
    // if (!main_map->collide(std::floor(player_position.x),
    //                        std::floor(player_position.y))) {
    //     main_player->set_position(player_position);
    // }
    // main_player->rotate_camera(c, diff_time);
    // main_renderer->clear_with(sdl::color{0x28, 0x2c, 0x44});

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    main_player->render_view(*main_map, surface_ids, window_width, window_height, shader_program);
    // main_renderer->present();
    SDL_GL_SwapWindow(main_window->get());
}

game::~game() {
    glDeleteProgram(shader_program);
    glDeleteTextures(surface_ids.size(), surface_ids.data());
}

void game::on_keyboard_event(const SDL_KeyboardEvent &event) {
    if (event.type == SDL_KEYUP && event.keysym.sym == SDLK_ESCAPE) {
        sdl::set_relative_mouse_mode(false);
        mouse_is_trapped = false;
    }
}

void game::on_mouse_button_event(const SDL_MouseButtonEvent &event) {
    if (event.type == SDL_MOUSEBUTTONUP) {
        if (!mouse_is_trapped) {
            sdl::set_relative_mouse_mode(true);
            mouse_is_trapped = true;
        } else if (event.button == SDL_BUTTON_LEFT) {
            main_player->current_inventory.trigger_selection(*main_map, *main_player);
        }
    }
}

void game::on_mouse_motion_event(const SDL_MouseMotionEvent &event) {
    if (mouse_is_trapped) {
        // main_camera->rotate(event.xrel * 0.003, event.yrel * -0.003);
        main_player->camera.rotate(event.xrel * 0.003, event.yrel * -0.003);
    }
}

void game::on_mouse_wheel_event(const SDL_MouseWheelEvent &event) {
    auto &inventory = main_player->current_inventory;
    if (!inventory.items.empty()) {
        inventory.selection += -event.y + static_cast<int>(inventory.items.size());
        inventory.selection %= inventory.items.size();
    }
}