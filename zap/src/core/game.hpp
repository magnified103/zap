#ifndef CORE_GAME_HPP
#define CORE_GAME_HPP

#include <memory>

// #include "scenery/camera.hpp"
#include "scenery/camera3d.hpp"
// #include "scenery/map.hpp"
#include "entity/player3d.hpp"
#include "opengl/gl.hpp"
#include "scenery/map3d.hpp"
// #include "scenery/player.hpp"
#include "physics/physics.hpp"
#include "timer/timer.hpp"
#include "sdl2/SDL.hpp"
#include "basic_game.hpp"

class game final : public basic_game {
public:
    game(const std::string &map_path);
    ~game() override;

protected:
    int window_width;
    int window_height;

    std::unique_ptr<player3d> main_player;
    // std::unique_ptr<camera> main_camera;
    // std::unique_ptr<camera3d> main_camera;
    // std::unique_ptr<map_grid> main_map;
    std::unique_ptr<map3d> main_map;

    std::vector<GLuint> surface_ids;

    GLuint shader_program;

    std::unique_ptr<sdl::window> main_window;
    // std::unique_ptr<sdl::renderer> main_renderer;

    std::unique_ptr<physics> physics_engine;

    const Uint8 *keyboard_state;

    timer game_timer;

    bool mouse_is_trapped;

    void game_logic() override;
    // void on_quit(const SDL_QuitEvent &event) override;
    // void on_window_event(const SDL_WindowEvent &event) override;
    void on_keyboard_event(const SDL_KeyboardEvent &event) override;
    void on_mouse_button_event(const SDL_MouseButtonEvent &event) override;
    void on_mouse_motion_event(const SDL_MouseMotionEvent &event) override;
    void on_mouse_wheel_event(const SDL_MouseWheelEvent &event) override;
};

#endif // CORE_GAME_HPP