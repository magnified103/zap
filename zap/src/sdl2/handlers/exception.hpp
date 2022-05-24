#ifndef SDL2_EXCEPTION_HPP
#define SDL2_EXCEPTION_HPP

#include <exception>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace sdl {

class exception : public std::exception {
public:
    exception() noexcept : msg("?"){};

    explicit exception(std::string what) noexcept : msg(what.empty() ? "?" : what) {}

    const char *what() const noexcept override { return msg.c_str(); }

private:
    std::string msg;
};

class sdl_exception final : public exception {
public:
    sdl_exception() noexcept : exception(SDL_GetError()) {}

    explicit sdl_exception(std::string what) noexcept : exception(what) {}
};

class img_exception final : public exception {
public:
    img_exception() noexcept : exception(IMG_GetError()) {}

    explicit img_exception(std::string what) noexcept : exception(what) {}
};

class ttf_exception final : public exception {
public:
    ttf_exception() noexcept : exception(TTF_GetError()) {}

    explicit ttf_exception(std::string what) noexcept : exception(what) {}
};

} // namespace sdl

#endif // SDL2_EXCEPTION_HPP