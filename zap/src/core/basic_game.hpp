#ifndef CORE_BASIC_GAME_HPP
#define CORE_BASIC_GAME_HPP

#include "sdl2/SDL.hpp"

class basic_game {
public:
    void run() {
        dispatcher.bind<SDL_QuitEvent>().to<&basic_game::on_quit>(this);
        dispatcher.bind<SDL_WindowEvent>().to<&basic_game::on_window_event>(this);
        dispatcher.bind<SDL_KeyboardEvent>().to<&basic_game::on_keyboard_event>(this);
        dispatcher.bind<SDL_MouseButtonEvent>().to<&basic_game::on_mouse_button_event>(this);
        dispatcher.bind<SDL_MouseMotionEvent>().to<&basic_game::on_mouse_motion_event>(this);
        dispatcher.bind<SDL_MouseWheelEvent>().to<&basic_game::on_mouse_wheel_event>(this);
        while (game_is_running) {
            dispatcher.poll();
            game_logic();
        }
    }

    virtual ~basic_game() = default;

protected:
    sdl::event_dispatcher<SDL_QuitEvent, SDL_WindowEvent, SDL_KeyboardEvent, SDL_MouseButtonEvent,
                          SDL_MouseMotionEvent, SDL_MouseWheelEvent>
        dispatcher;
    bool game_is_running{true};

    virtual void game_logic() {}
    virtual void on_quit(const SDL_QuitEvent &event) { game_is_running = false; }
    virtual void on_window_event(const SDL_WindowEvent &event) {}
    virtual void on_keyboard_event(const SDL_KeyboardEvent &event) {}
    virtual void on_mouse_button_event(const SDL_MouseButtonEvent &event) {}
    virtual void on_mouse_motion_event(const SDL_MouseMotionEvent &event) {}
    virtual void on_mouse_wheel_event(const SDL_MouseWheelEvent &event) {}
};

#endif // CORE_BASIC_GAME_HPP