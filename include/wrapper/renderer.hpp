#ifndef WRAPPER_RENDERER_HPP
#define WRAPPER_RENDERER_HPP

#include <SDL2/SDL.h>

#include "color.hpp"
#include "common.hpp"
#include "exception.hpp"
#include "geometry.hpp"
#include "surface.hpp"
#include "texture.hpp"

namespace sdl {

template <class traits>
class basic_renderer;

using renderer = basic_renderer<detail::owner_tag>;
using renderer_handle = basic_renderer<detail::handle_tag>;

template <class traits>
class basic_renderer {
public:
    basic_renderer(SDL_Renderer *renderer) : sdl_renderer{renderer} {
        if constexpr (detail::is_owner<traits>) {
            if (!sdl_renderer) {
                throw exception("Cannot create renderer from nullptr");
            }
        }
    }

    template <class T = traits, detail::enable_for_handle<T> = 0>
    basic_renderer(const renderer &other) : sdl_renderer{other.get()} {}

    ~basic_renderer() {
        if constexpr (detail::is_owner<traits>) {
            SDL_DestroyRenderer(sdl_renderer);
        }
    }

    SDL_Renderer *get() const noexcept { return sdl_renderer; }

    template <class T>
    texture create_texture(const basic_surface<T> &surface) const {
        auto *ptr = SDL_CreateTextureFromSurface(sdl_renderer, surface.get());
        if (ptr) {
            return texture{ptr};
        }
        throw sdl_exception();
    }

    void present() { SDL_RenderPresent(sdl_renderer); }

    template <class value_type>
    int draw_rect(basic_rect<value_type> rect) {
        if constexpr (rect.is_integral) {
            return SDL_RenderDrawRect(sdl_renderer, rect.get_ptr());
        } else {
            return SDL_RenderDrawRectF(sdl_renderer, rect.get_ptr());
        }
    }

    template <class value_type>
    int fill_rect(basic_rect<value_type> rect) {
        if constexpr (rect.is_integral) {
            return SDL_RenderFillRect(sdl_renderer, rect.get_ptr());
        } else {
            return SDL_RenderFillRectF(sdl_renderer, rect.get_ptr());
        }
    }

    int set_draw_color(color color) {
        return SDL_SetRenderDrawColor(sdl_renderer, color.red(), color.green(),
                                      color.blue(), color.alpha());
    }

    color get_draw_color() {
        color draw_color;
        if (SDL_GetRenderDrawColor(sdl_renderer, &draw_color.red(),
                                   &draw_color.green(), &draw_color.blue(),
                                   &draw_color.alpha()) < 0) {
            throw sdl_exception();
        }
        return draw_color;
    }

    template <class T, class value_type>
    int render(const basic_texture<T> &texture,
               const basic_rect<value_type> &dest) {
        if constexpr (dest.is_integral) {
            return SDL_RenderCopy(sdl_renderer, texture.get(), nullptr,
                                  dest.get_ptr());
        } else {
            return SDL_RenderCopyF(sdl_renderer, texture.get(), nullptr,
                                   dest.get_ptr());
        }
    }

    template <class T>
    int render(const basic_texture<T> &texture) {
        return SDL_RenderCopy(sdl_renderer, texture.get(), nullptr, nullptr);
    }

    int fill() { return SDL_RenderFillRect(sdl_renderer, nullptr); }

    void fill_with(const color &color) {
        const auto previous = get_draw_color();
        set_draw_color(color);
        fill();
        set_draw_color(previous);
    }

    int clear() { return SDL_RenderClear(sdl_renderer); }

    void clear_with(const color &color) {
        const auto previous = get_draw_color();
        set_draw_color(color);
        clear();
        set_draw_color(previous);
    }

private:
    SDL_Renderer *sdl_renderer;
};

} // namespace sdl

#endif // WRAPPER_RENDERER_HPP