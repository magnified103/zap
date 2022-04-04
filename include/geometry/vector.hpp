#ifndef GEOMETRY_VECTOR_HPP
#define GEOMETRY_VECTOR_HPP

template <int L>
struct vec;

using vec2 = vec<2>;
using vec3 = vec<3>;
using vec4 = vec<4>;

template <>
struct vec<2> {
    float x;
    float y;
    vec() = default;
    explicit vec(float scalar);
    vec(float x, float y);
};

template <>
struct vec<3> {
    float x;
    float y;
    float z;
    vec() = default;
    explicit vec(float scalar);
    vec(float x, float y, float z);
};

template <>
struct vec<4> {
    float x;
    float y;
    float z;
    float w;
    vec() = default;
    explicit vec(float scalar);
    vec(float x, float y, float z, float w);
    float &operator[](int i);
    float const &operator[](int i) const;
};

inline float &vec<4>::operator[](int i) {
    switch (i) {
    default:
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    }
}

inline float const &vec<4>::operator[](int i) const {
    switch (i) {
    default:
    case 0:
        return x;
    case 1:
        return y;
    case 2:
        return z;
    case 3:
        return w;
    }
}

inline vec<2>::vec(float scalar) : x(scalar), y(scalar) {}

inline vec<3>::vec(float scalar) : x(scalar), y(scalar), z(scalar) {}

inline vec<4>::vec(float scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}

inline vec<2>::vec(float x, float y) : x(x), y(y) {}

inline vec<3>::vec(float x, float y, float z) : x(x), y(y), z(z) {}

inline vec<4>::vec(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w) {}

inline vec<2> operator+(const vec<2> &a, const vec<2> &b) {
    return vec<2>{a.x + b.x, a.y + b.y};
}

inline vec<3> operator+(const vec<3> &a, const vec<3> &b) {
    return vec<3>{a.x + b.x, a.y + b.y, a.z + b.z};
}

inline vec<4> operator+(const vec<4> &a, const vec<4> &b) {
    return vec<4>{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

inline vec<2> operator-(const vec<2> &a, const vec<2> &b) {
    return vec<2>{a.x - b.x, a.y - b.y};
}

inline vec<3> operator-(const vec<3> &a, const vec<3> &b) {
    return vec<3>{a.x - b.x, a.y - b.y, a.z - b.z};
}

inline vec<4> operator-(const vec<4> &a, const vec<4> &b) {
    return vec<4>{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

inline vec<2> operator*(float scalar, const vec<2> &v) {
    return vec<2>{scalar * v.x, scalar * v.y};
}

inline vec<3> operator*(float scalar, const vec<3> &v) {
    return vec<3>{scalar * v.x, scalar * v.y, scalar * v.z};
}

inline vec<4> operator*(float scalar, const vec<4> &v) {
    return vec<4>{scalar * v.x, scalar * v.y, scalar * v.z, scalar * v.w};
}

inline vec<2> operator*(const vec<2> &v, float scalar) {
    return vec<2>{scalar * v.x, scalar * v.y};
}

inline vec<3> operator*(const vec<3> &v, float scalar) {
    return vec<3>{scalar * v.x, scalar * v.y, scalar * v.z};
}

inline vec<4> operator*(const vec<4> &v, float scalar) {
    return vec<4>{scalar * v.x, scalar * v.y, scalar * v.z, scalar * v.w};
}

inline vec<4> operator*(const vec<4> &a, const vec<4> &b) {
    return vec<4>{a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

inline float dot(const vec<2> &a, const vec<2> &b) {
    return a.x * b.x + a.y * b.y;
}

inline float dot(const vec<3> &a, const vec<3> &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float dot(const vec<4> &a, const vec<4> &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline vec<3> cross(const vec<3> &x, const vec<3> &y) {
    return vec<3>{x.y * y.z - y.y * x.z, x.z * y.x - y.z * x.x,
                  x.x * y.y - y.x * x.y};
}

template <int L>
inline vec<L> normalize(const vec<L> &v) {
    return static_cast<float>(1) / std::sqrt(dot(v, v)) * v;
}

#endif // GEOMETRY_VECTOR_HPP