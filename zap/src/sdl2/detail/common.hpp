#ifndef SDL2_COMMON_HPP
#define SDL2_COMMON_HPP

#include <type_traits>

#include <SDL.h>

namespace sdl {

using int8 = Sint8;
using int16 = Sint16;
using int32 = Sint32;
using int64 = Sint64;

using uint8 = Uint8;
using uint16 = Uint16;
using uint32 = Uint32;
using uint64 = Uint64;

} // namespace sdl

namespace sdl::detail {

struct owner_tag {};
struct handle_tag {};

template <typename T>
inline constexpr bool is_owner = std::is_same_v<T, owner_tag>;

template <typename T>
inline constexpr bool is_handle = std::is_same_v<T, handle_tag>;

template <typename T>
using enable_for_owner = std::enable_if_t<is_owner<T>, int>;

template <typename T>
using enable_for_handle = std::enable_if_t<is_handle<T>, int>;

} // namespace sdl::detail

#endif // SDL2_COMMON_HPP