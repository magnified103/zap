#include <fstream>
#include <memory>
#include <string>

#include "core/game.hpp"
// #include "../../include/geometry/point.hpp"
#include "geometry/trigonometric.hpp"
#include "geometry/vector.hpp"
#include "opengl/context.hpp"
#include "opengl/gl.hpp"
#include "opengl/utilities.hpp"
#include "scenery/block.hpp"
#include "scenery/tile.hpp"
#include "wrapper/SDL.hpp"

void GL_APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                 GLenum severity, GLsizei length,
                                 const GLchar *message, const void *userParam) {
    if (type == GL_DEBUG_TYPE_ERROR_KHR) {
        throw std::runtime_error(message);
    } else {
        sdl::log_info("Callback type=0x%x, severity=0x%x, message=%s", type,
                      severity, message);
    }
}

void game::initialize() {

    // specify window sizes
    window_width = 800;
    window_height = 640;

    // create window
    main_window = std::make_unique<sdl::window>(
        "zap", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width,
        window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    context_loader context(main_window->get());

    // load map
    main_map = std::make_unique<map3d>();
    load_map_and_textures("map.json", *main_map, surface_ids);

    // vsync
    // if (sdl::gl_set_swap_interval(1) < 0) {
    //     throw std::runtime_error("Unable to turn VSync on");
    // }

    // debug trap
    glEnable(GL_DEBUG_OUTPUT_KHR);
    glDebugMessageCallbackKHR(MessageCallback, 0);

    // w-component depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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

    // // keyboard state
    // keyboard_state = SDL_GetKeyboardState(nullptr);

    // camera
    main_camera = std::make_unique<camera3d>(
        camera3d{radians(60.0f), 0.5f, 1000.0f, 0.0f, radians(90.0f)});

    mouse_is_trapped = false;

    // // trap cursor
    // sdl::set_relative_mouse_mode(true);
    // mouse_is_trapped = true;

    shader_program = load_shader_program(R"(#version 300 es

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
                                         R"(#version 300 es

    precision highp float;

    // Interpolated values from the vertex shaders
    in vec2 UV;

    // Ouput data
    out vec3 color;

    // Values that stay constant for the whole mesh.
    uniform sampler2D texture_sampler;

    void main() {
        // Output color = color of the texture at the specified UV
        color = texture( texture_sampler, UV ).rgb;
    })");

    // timer start
    game_timer.start();
    // main_map->number_of_columns = 1;
    // main_map->number_of_rows = 1;
    // main_map->cell_size = 5.0f;
    // main_map->terrain = {{{0, -1, -1, {0, 0, 0}, {5, 20, 5}, 1}}};
    // main_map->blocks = {{-1, -1, -1, -1, -1, 0, 20, 15}};
    // main_map->textures = {
    //     {0,
    //      {{0, 0, 0}, {0, 0, 5}, {5, 0, 0}, {0, 0, 5}, {5, 0, 0}, {5, 0, 5}},
    //      {{0, 0}, {0, 1}, {1, 0}, {0, 1}, {1, 0}, {1, 1}}}};
    // main_map->surfaces = {"./res/plain-white.png"};
    // save_map("map.json", *main_map);

    // load_textures("map.json", *main_map, surface_ids);

    // default clear color
    glClearColor(0, 0, 0, 1);
}

void game::game_logic() {
    Uint32 diff_time = game_timer.get_ticks();
    game_timer.start();
    // main_window->set_title("zap - fps=" + std::to_string(1000.0f /
    // diff_time)); int move_type = movement_type::none; if (mouse_is_trapped) {
    //     if (keyboard_state[SDL_SCANCODE_W]) {
    //         move_type = movement_type::up;
    //     } else if (keyboard_state[SDL_SCANCODE_S]) {
    //         move_type = movement_type::down;
    //     }
    // }
    float c = 0;
    // if (!mouse_is_trapped) {
    //     if (keyboard_state[SDL_SCANCODE_A]) {
    //         c = -1;
    //     } else if (keyboard_state[SDL_SCANCODE_D]) {
    //         c = 1;
    //     }
    // }
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
    main_camera->render(vec3{0, 0, 0}, vec3{0, 1, 0}, *main_map, surface_ids,
                        window_width, window_height, shader_program);
    // main_renderer->present();
    SDL_GL_SwapWindow(main_window->get());
}

int game::cleanup() {
    glDeleteProgram(shader_program);
    return 0;
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
        }
    }
}

void game::on_mouse_motion_event(const SDL_MouseMotionEvent &event) {
    if (mouse_is_trapped) {
        main_camera->rotate(event.xrel * 0.003, event.yrel * -0.003);
    }
}

void game::on_mouse_wheel_event(const SDL_MouseWheelEvent &event) {}