#ifndef GEOMETRY_UTILITIES_HPP
#define GEOMETRY_UTILITIES_HPP

#include <cmath>

template <class value_type>
struct epsilon {
    inline constexpr static value_type value;
};

inline constexpr float epsilon<float>::value = 1e-9;

template <class value_type>
inline constexpr bool approximately_equal(const value_type &a,
                                          const value_type &b) {
    return abs(a - b) <= std::max(abs(a), abs(b)) * epsilon<value_type>::value;
}

template <class value_type>
inline constexpr bool essentially_equal(const value_type &a,
                                        const value_type &b) {
    return abs(a - b) <= std::min(abs(a), abs(b)) * epsilon<value_type>::value;
}

#endif // GEOMETRY_UTILITIES_HPP