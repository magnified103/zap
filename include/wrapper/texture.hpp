#ifndef WRAPPER_TEXTURE_HPP
#define WRAPPER_TEXTURE_HPP

#include <SDL2/SDL.h>

#include "common.hpp"

namespace sdl {

template <class traits>
class basic_texture;

using texture = basic_texture<detail::owner_tag>;
using texture_handle = basic_texture<detail::handle_tag>;

template <class traits>
class basic_texture {
public:
    basic_texture() : sdl_texture(){};

    template <class T = traits, detail::enable_for_owner<T>>
    basic_texture(renderer_handle renderer, Uint32 format, int access, int w,
                  int h) {
        sdl_texture = SDL_CreateTexture(renderer.get(), format, access, w, h);
    }

    template <class T = traits, detail::enable_for_owner<T>>
    basic_texture(renderer_handle renderer, SDL_Surface *surface) {
        sdl_texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
    }

    template <class T = traits, detail::enable_for_handle<T>>
    basic_texture(const texture &other) : sdl_texture(other.get()) {}

    ~basic_texture() {
        if constexpr (detail::is_owner<traits>) {
            SDL_DestroyTexture(sdl_texture);
        }
    }

    SDL_Texture *get() const noexcept { return sdl_texture; }

private:
    SDL_Texture *sdl_texture;
};

} // namespace sdl

#endif // WRAPPER_TEXTURE_HPP