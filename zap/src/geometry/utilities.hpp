#ifndef GEOMETRY_UTILITIES_HPP
#define GEOMETRY_UTILITIES_HPP

#include <cmath>

inline constexpr float epsilon = 1e-8;

template <class value_type>
inline constexpr bool approximately_equal(const value_type &a,
                                          const value_type &b) {
    return std::abs(a - b) <= std::max(std::abs(a), std::abs(b)) * epsilon;
}

template <class value_type>
inline constexpr bool essentially_equal(const value_type &a,
                                        const value_type &b) {
    return std::abs(a - b) <= std::min(std::abs(a), std::abs(b)) * epsilon;
}

#endif // GEOMETRY_UTILITIES_HPP