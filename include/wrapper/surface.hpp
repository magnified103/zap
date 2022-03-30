#ifndef WRAPPER_SURFACE_HPP
#define WRAPPER_SURFACE_HPP

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "color.hpp"
#include "common.hpp"
#include "exception.hpp"
#include "geometry.hpp"

namespace sdl {

template <class traits>
class basic_surface;

using surface = basic_surface<detail::owner_tag>;
using surface_handle = basic_surface<detail::handle_tag>;

template <class traits>
class basic_surface {
public:
    basic_surface(SDL_Surface *window) : surface{surface} {
        if constexpr (detail::is_owner<traits>) {
            if (!surface) {
                throw exception("Cannot create surface from nullptr");
            }
        }
    }

    template <class T = traits, detail::enable_for_owner<T> = 0>
    basic_surface(std::string file) : surface(IMG_Load(file.c_str())) {
        if (!surface) {
            throw img_exception();
        }
    }

    int fill_rect(rect rect, color color) {
        return SDL_FillRect(surface, rect.get_ptr(),
                            SDL_MapRGBA(surface->format, color.red(),
                                        color.green(), color.blue(),
                                        color.alpha()));
    }

private:
    SDL_Surface *surface;
};

} // namespace sdl

#endif // WRAPPER_SURFACE_HPP