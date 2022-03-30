#ifndef WRAPPER_TEXTURE_HPP
#define WRAPPER_TEXTURE_HPP

#include <SDL2/SDL.h>

#include "common.hpp"
#include "exception.hpp"
#include "renderer.hpp"

namespace sdl {

template <class traits>
class basic_texture;

using texture = basic_texture<detail::owner_tag>;
using texture_handle = basic_texture<detail::handle_tag>;

template <class traits>
class basic_texture {
public:
    basic_texture(SDL_Texture *texture) : sdl_texture{texture} {
        if constexpr (detail::is_owner<traits>) {
            if (!texture) {
                throw exception("Cannot create texture from nullptr");
            }
        }
    }

    template <class T = traits, detail::enable_for_handle<T> = 0>
    basic_texture(const texture &other) : sdl_texture(other.get()) {}

    template <class T = traits, detail::enable_for_owner<T> = 0>
    basic_texture(texture &&other) : sdl_texture{other.get()} {}

    ~basic_texture() {
        if constexpr (detail::is_owner<traits>) {
            if (sdl_texture) {
                SDL_DestroyTexture(sdl_texture);
            }
        }
    }

    SDL_Texture *get() const noexcept { return sdl_texture; }

    int get_width() {
        int w;
        if (SDL_QueryTexture(sdl_texture, nullptr, nullptr, &w, nullptr) < 0) {
            throw sdl_exception();
        }
        return w;
    }

    int get_height() {
        int h;
        if (SDL_QueryTexture(sdl_texture, nullptr, nullptr, nullptr, &h) < 0) {
            throw sdl_exception();
        }
        return h;
    }

private:
    SDL_Texture *sdl_texture;
};

} // namespace sdl

#endif // WRAPPER_TEXTURE_HPP