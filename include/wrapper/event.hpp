#ifndef WRAPPER_EVENT_HPP
#define WRAPPER_EVENT_HPP

#include <functional>
#include <tuple>
#include <type_traits>
#include <variant>

#include <SDL2/SDL.h>

namespace sdl {

class event_handle {
public:
    event_handle() : type{SDL_LASTEVENT}, data{} {}

    int poll() {
        SDL_Event event;
        if (SDL_PollEvent(&event)) {
            store(event);
            return 1;
        }
        reset_state();
        return 0;
    }

    template <class T>
    T *get_if() {
        return std::get_if<T>(&data);
    }

private:
    using data_type = std::variant<
        std::monostate, SDL_WindowEvent, SDL_KeyboardEvent,
        SDL_TextEditingEvent, SDL_TextInputEvent, SDL_MouseMotionEvent,
        SDL_MouseButtonEvent, SDL_MouseWheelEvent, SDL_JoyAxisEvent,
        SDL_JoyBallEvent, SDL_JoyHatEvent, SDL_JoyButtonEvent,
        SDL_JoyDeviceEvent, SDL_ControllerAxisEvent, SDL_ControllerButtonEvent,
        SDL_ControllerDeviceEvent, SDL_AudioDeviceEvent, SDL_QuitEvent,
        SDL_UserEvent, SDL_SysWMEvent, SDL_TouchFingerEvent,
        SDL_MultiGestureEvent, SDL_DollarGestureEvent, SDL_DropEvent>;

    SDL_EventType type;
    data_type data;

    void reset_state() {
        type = SDL_LASTEVENT;
        data.emplace<std::monostate>();
    }

    void store(SDL_Event event) {
        type = static_cast<SDL_EventType>(event.type);
        switch (type) {
        case SDL_FIRSTEVENT:
        case SDL_LASTEVENT:
            reset_state();
            break;
        case SDL_QUIT:
            data.emplace<SDL_QuitEvent>(event.quit);
            break;
        case SDL_WINDOWEVENT:
            data.emplace<SDL_WindowEvent>(event.window);
            break;
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            data.emplace<SDL_KeyboardEvent>(event.key);
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            data.emplace<SDL_MouseButtonEvent>(event.button);
            break;
        default:
            reset_state();
            break;
        }
    }
};

template <class event_type>
class event_callback_handle {
public:
    using function_type = std::function<void(const event_type &)>;

    void reset() { callback = nullptr; }

    template <class func>
    void to(func &&callable) {
        static_assert(std::is_invocable_v<func, const event_type &>);
        callback = callable;
    }

    template <auto func, class obj>
    void to(obj *self) {
        static_assert(std::is_member_function_pointer_v<decltype(func)>);
        static_assert(
            std::is_invocable_v<decltype(func), obj *, const event_type &>);
        to(std::bind(func, self, std::placeholders::_1));
    }

    template <auto func>
    void to() {
        to(func);
    }

    function_type &get_callback() { return callback; }

private:
    function_type callback;
};

template <class... event_types>
class event_dispatcher {
public:
    static_assert((!std::is_const_v<event_types> && ...));
    static_assert((!std::is_volatile_v<event_types> && ...));
    static_assert((!std::is_reference_v<event_types> && ...));
    static_assert((!std::is_pointer_v<event_types> && ...));

    template <class event_type>
    event_callback_handle<event_type> &bind() {
        return std::get<event_callback_handle<event_type>>(callbacks);
    }

    template <class event_type>
    bool invoke() {
        const auto *event = handle.get_if<event_type>();
        if (!event) {
            return false;
        }
        auto &callback = bind<event_type>().get_callback();
        if (callback) {
            callback(*event);
        }
        return true;
    }

    void poll() {
        while (handle.poll()) {
            (invoke<event_types>() || ...);
        }
    }

    void reset() { (bind<event_types>().reset(), ...); }

private:
    using callback_tuple = std::tuple<event_callback_handle<event_types>...>;

    event_handle handle;
    callback_tuple callbacks;
};

} // namespace sdl

#endif // WRAPPER_EVENT_HPP