#ifndef CORE_GAME_HPP
#define CORE_GAME_HPP

#include <memory>

#include "../scenery/camera.hpp"
#include "../scenery/map.hpp"
#include "../scenery/player.hpp"
#include "../timer/timer.hpp"
#include "../wrapper/SDL.hpp"
#include "basic_game.hpp"

class game final : public basic_game {
public:
protected:
    int window_width;
    int window_height;

    std::unique_ptr<player> main_player;
    std::unique_ptr<camera> main_camera;
    std::unique_ptr<map_grid> main_map;

    std::unique_ptr<sdl::window> main_window;
    std::unique_ptr<sdl::renderer> main_renderer;

    const Uint8 *keyboard_state;

    timer game_timer;

    bool mouse_is_trapped;

    void initialize() override;
    void game_logic() override;
    // void on_quit(const SDL_QuitEvent &event) override;
    // void on_window_event(const SDL_WindowEvent &event) override;
    void on_keyboard_event(const SDL_KeyboardEvent &event) override;
    void on_mouse_button_event(const SDL_MouseButtonEvent &event) override;
    void on_mouse_motion_event(const SDL_MouseMotionEvent &event) override;
};

#endif // CORE_GAME_HPP