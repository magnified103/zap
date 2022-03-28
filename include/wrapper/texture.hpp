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
    basic_texture() : texture(){};

    template <class T = traits, detail::enable_for_owner<T>>
    basic_texture(renderer_handle renderer, Uint32 format, int access, int w,
                  int h) {
        texture = SDL_CreateTexture(renderer.get(), format, access, w, h);
    }

    template <class T = traits, detail::enable_for_owner<T>>
    basic_texture(renderer_handle renderer, SDL_Surface *surface) {
        texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
    }

    template <class T = traits, detail::enable_for_handle<T>>
    basic_texture(const texture &other) : texture(other.texture) {}

    ~basic_texture() {
        if constexpr (detail::is_owner<traits>) {
            SDL_DestroyTexture(texture);
        }
    }

private:
    SDL_Texture *texture;
};

}

#endif // WRAPPER_TEXTURE_HPP