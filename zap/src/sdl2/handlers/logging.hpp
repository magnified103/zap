#ifndef SDL2_LOGGING_HPP
#define SDL2_LOGGING_HPP

#include <cassert>
#include <utility>

#include <SDL.h>

#if (SDL_MAJOR_VERSION == 2) && (SDL_MINOR_VERSION == 0) &&                    \
    (SDL_PATCHLEVEL == 10)

using SDL_LogCategory = decltype(SDL_LOG_CATEGORY_APPLICATION);

#endif

namespace sdl {

template <class... Args>
void log(SDL_LogPriority priority, SDL_LogCategory category, const char *fmt,
         Args &&...args) {
    assert(fmt);
    SDL_LogMessage(category, priority, fmt, std::forward<Args>(args)...);
}

template <class... Args>
void log_verbose(const char *fmt, Args &&...args) {
    log(SDL_LOG_PRIORITY_VERBOSE, SDL_LOG_CATEGORY_APPLICATION, fmt,
        std::forward<Args>(args)...);
}

template <class... Args>
void log_debug(const char *fmt, Args &&...args) {
    log(SDL_LOG_PRIORITY_DEBUG, SDL_LOG_CATEGORY_APPLICATION, fmt,
        std::forward<Args>(args)...);
}

template <class... Args>
void log_info(const char *fmt, Args &&...args) {
    log(SDL_LOG_PRIORITY_INFO, SDL_LOG_CATEGORY_APPLICATION, fmt,
        std::forward<Args>(args)...);
}

template <class... Args>
void log_warn(const char *fmt, Args &&...args) {
    log(SDL_LOG_PRIORITY_WARN, SDL_LOG_CATEGORY_APPLICATION, fmt,
        std::forward<Args>(args)...);
}

template <class... Args>
void log_error(const char *fmt, Args &&...args) {
    log(SDL_LOG_PRIORITY_ERROR, SDL_LOG_CATEGORY_APPLICATION, fmt,
        std::forward<Args>(args)...);
}

template <class... Args>
void log_critical(const char *fmt, Args &&...args) {
    log(SDL_LOG_PRIORITY_CRITICAL, SDL_LOG_CATEGORY_APPLICATION, fmt,
        std::forward<Args>(args)...);
}

} // namespace sdl

#endif // SDL2_LOGGING_HPP