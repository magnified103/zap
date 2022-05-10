#ifndef GEOMETRY_TRIGONOMETRIC_HPP
#define GEOMETRY_TRIGONOMETRIC_HPP

// radians
template <typename genType>
genType radians(genType degrees) {
    static_assert(std::numeric_limits<genType>::is_iec559,
                  "'radians' only accept floating-point input");

    return degrees * static_cast<genType>(0.01745329251994329576923690768489);
}

// degrees
template <typename genType>
genType degrees(genType radians) {
    static_assert(std::numeric_limits<genType>::is_iec559,
                  "'degrees' only accept floating-point input");

    return radians * static_cast<genType>(57.295779513082320876798154814105);
}

#endif // GEOMETRY_TRIGONOMETRIC_HPP