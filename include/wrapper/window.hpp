#ifndef WRAPPER_WINDOW_HPP
#define WRAPPER_WINDOW_HPP

#include <string>

#include <SDL2/SDL.h>

#include "common.hpp"
#include "exception.hpp"
#include "renderer.hpp"

namespace sdl {

template <class traits>
class basic_window;

using window = basic_window<detail::owner_tag>;
using window_handle = basic_window<detail::handle_tag>;

template <class traits>
class basic_window {
public:
    basic_window(SDL_Window *window) : window{window} {
        if constexpr (detail::is_owner<traits>) {
            if (!window) {
                throw exception("Cannot create window from nullptr");
            }
        }
    }

    template <class T = traits, detail::enable_for_owner<T> = 0>
    basic_window(std::string title, int x, int y, int w, int h, Uint32 flags) {
        window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
    }

    template <class T = traits, detail::enable_for_handle<T> = 0>
    basic_window(const window &other) : window{other.get()} {}

    ~basic_window() {
        if constexpr (detail::is_owner<traits>) {
            SDL_DestroyWindow(window);
        }
    }

    SDL_Window *get() const noexcept { return window; }

    renderer create_renderer(int index, Uint32 flags) {
        auto *ptr = SDL_CreateRenderer(window, index, flags);
        if (ptr) {
            return renderer(ptr);
        }
        throw sdl_exception();
    }

    renderer create_renderer(Uint32 flags) {
        return create_renderer(-1, flags);
    }

    renderer_handle get_renderer() {
        return renderer_handle(SDL_GetRenderer(window));
    }

private:
    SDL_Window *window;
};

} // namespace sdl

#endif // WRAPPER_WINDOW_HPP