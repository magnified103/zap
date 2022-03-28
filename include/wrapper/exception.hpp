#ifndef WRAPPER_EXCEPTION_HPP
#define WRAPPER_EXCEPTION_HPP

#include <exception>
#include <string>

#include <SDL2/SDL.h>

namespace sdl {

class exception : public std::exception {
public:
    exception() noexcept : msg("?"){};

    explicit exception(std::string what) noexcept
        : msg(what.empty() ? "?" : what) {}

    const char *what() const noexcept override { return msg.c_str(); }

private:
    std::string msg;
};

class sdl_exception final : public exception {
public:
    sdl_exception() noexcept : exception(SDL_GetError()) {}

    explicit sdl_exception(std::string what) noexcept : exception(what) {}
};

}

#endif // WRAPPER_EXCEPTION_HPP