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
    basic_window(SDL_Window *window) : sdl_window{window} {
        if constexpr (detail::is_owner<traits>) {
            if (!sdl_window) {
                throw exception("Cannot create window from nullptr");
            }
        }
    }

    template <class T = traits, detail::enable_for_owner<T> = 0>
    basic_window(std::string title, int x, int y, int w, int h, Uint32 flags) {
        sdl_window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
    }

    template <class T = traits, detail::enable_for_owner<T> = 0>
    basic_window(std::string title, int w, int h) {
        sdl_window =
            SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    }

    template <class T = traits, detail::enable_for_handle<T> = 0>
    basic_window(const window &other) : sdl_window{other.get()} {}

    ~basic_window() {
        if constexpr (detail::is_owner<traits>) {
            SDL_DestroyWindow(sdl_window);
        }
    }

    SDL_Window *get() const noexcept { return sdl_window; }

    renderer_handle create_renderer(int index, Uint32 flags) {
        auto *ptr = SDL_CreateRenderer(sdl_window, index, flags);
        if (ptr) {
            return renderer_handle(ptr);
        }
        throw sdl_exception();
    }

    renderer_handle create_renderer(Uint32 flags) {
        return create_renderer(-1, flags);
    }

    renderer_handle get_renderer() {
        return renderer_handle(SDL_GetRenderer(sdl_window));
    }

    std::string get_title() { return SDL_GetWindowTitle(sdl_window); }

    void set_title(std::string title) {
        SDL_SetWindowTitle(sdl_window, title.c_str());
    }

private:
    SDL_Window *sdl_window;
};

} // namespace sdl

#endif // WRAPPER_WINDOW_HPP