#ifndef SDL2_SURFACE_HPP
#define SDL2_SURFACE_HPP

#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "sdl2/detail/common.hpp"
#include "sdl2/handlers/exception.hpp"
#include "sdl2/types/color.hpp"
#include "sdl2/types/geometry.hpp"

namespace sdl {

template <class traits>
class basic_surface;

using surface = basic_surface<detail::owner_tag>;
using surface_handle = basic_surface<detail::handle_tag>;

template <class traits>
class basic_surface {
public:
    basic_surface(SDL_Surface *surface) : sdl_surface{surface} {
        if constexpr (detail::is_owner<traits>) {
            if (!sdl_surface) {
                throw exception("Cannot create surface from nullptr");
            }
        }
    }

    basic_surface(int width, int height) : sdl_surface {
        SDL_CreateRGBSurface(0, width, height, 32,
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
                             0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff
#else
                             0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000
#endif
        )
    }
    {
        if (!sdl_surface) {
            throw sdl_exception();
        }
    }

    template <class T = traits, detail::enable_for_owner<T> = 0>
    basic_surface(std::string file) : sdl_surface{IMG_Load(file.c_str())} {
        if (!sdl_surface) {
            throw img_exception();
        }
    }

    template <class T = traits, detail::enable_for_handle<T> = 0>
    basic_surface(const surface &other) : sdl_surface(other.get()) {}

    ~basic_surface() {
        if constexpr (detail::is_owner<traits>) {
            SDL_FreeSurface(sdl_surface);
        }
    }

    SDL_Surface *get() const noexcept { return sdl_surface; }

    int fill_rect(const rect &rect, const color &color) {
        return SDL_FillRect(sdl_surface, rect.get_ptr(),
                            SDL_MapRGBA(sdl_surface->format, color.red(), color.green(),
                                        color.blue(), color.alpha()));
    }

    int fill(const color &color) {
        return SDL_FillRect(sdl_surface, nullptr,
                            SDL_MapRGBA(sdl_surface->format, color.red(), color.green(),
                                        color.blue(), color.alpha()));
    }

private:
    SDL_Surface *sdl_surface;
};

} // namespace sdl

#endif // SDL2_SURFACE_HPP