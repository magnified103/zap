#ifndef SDL2_FONT_HPP
#define SDL2_FONT_HPP

#include <SDL.h>
#include <SDL_ttf.h>

#include "sdl2/detail/common.hpp"
#include "sdl2/handlers/exception.hpp"
#include "sdl2/types/color.hpp"
#include "sdl2/types/surface.hpp"

namespace sdl {

template <class traits>
class basic_font;

using font = basic_font<detail::owner_tag>;
using font_handle = basic_font<detail::handle_tag>;

template <class traits>
class basic_font {
public:
    basic_font(TTF_Font *font) : ttf_font{font} {
        if constexpr (detail::is_owner<traits>) {
            if (!font) {
                throw exception("Cannot create font from nullptr");
            }
        }
    }

    template <class T = traits, detail::enable_for_owner<T> = 0>
    basic_font(const std::string &file, const int &ptsize)
        : ttf_font{TTF_OpenFont(file.c_str(), ptsize)} {
        if (!ttf_font) {
            throw ttf_exception();
        }
    }

    template <class T = traits, detail::enable_for_handle<T> = 0>
    basic_font(const font &other) : ttf_font(other.get()) {}

    ~basic_font() {
        if constexpr (detail::is_owner<traits>) {
            if (ttf_font) {
                TTF_CloseFont(ttf_font);
            }
        }
    }

    TTF_Font *get() const noexcept { return ttf_font; }

    int get_style() const {
        if (ttf_font) {
            return TTF_GetFontStyle(ttf_font);
        }
        throw exception("Cannot get style from nullptr font");
    }

    void set_style(const int &style) {
        if (ttf_font) {
            return TTF_SetFontStyle(ttf_font, style);
        }
        throw exception("Cannot set style from nullptr font");
    }

    surface render_utf8_solid(const std::string &text, const color &fg) const {
        if (ttf_font) {
            return surface{TTF_RenderUTF8_Solid(ttf_font, text.c_str(), fg.get())};
        }
        throw exception("Cannot render solid UTF8 from nullptr font");
    }

    surface render_utf8_shaded(const std::string &text, const color &fg, const color &bg) const {
        if (ttf_font) {
            return surface{TTF_RenderUTF8_Shaded(ttf_font, text.c_str(), fg.get(), bg.get())};
        }
        throw exception("Cannot render shaded UTF8 from nullptr font");
    }

    surface render_utf8_blended(const std::string &text, const color &fg) const {
        if (ttf_font) {
            return surface{TTF_RenderUTF8_Blended(ttf_font, text.c_str(), fg.get())};
        }
        throw exception("Cannot render blended UTF8 from nullptr font");
    }

private:
    TTF_Font *ttf_font;
};

} // namespace sdl

#endif // SDL2_FONT_HPP