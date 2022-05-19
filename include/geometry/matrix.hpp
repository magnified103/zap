#ifndef GEOMETRY_MATRIX_HPP
#define GEOMETRY_MATRIX_HPP

#include <cassert>
#include <cmath>
#include <limits>

#include "vector.hpp"

template <int C, int R>
struct mat;

using mat3 = mat<3, 3>;
using mat4 = mat<4, 4>;

template <>
struct mat<4, 4> {
    vec<4> value[4];
    mat() = default;
    mat(float m);
    mat(vec<4> const &v0, vec<4> const &v1, vec<4> const &v2, vec<4> const &v3);
    vec<4> &operator[](int i);
    vec<4> const &operator[](int i) const;
};

inline mat<4, 4>::mat(float m)
    : value{vec<4>{m, 0, 0, 0}, vec<4>{0, m, 0, 0}, vec<4>{0, 0, m, 0},
            vec<4>{0, 0, 0, m}} {}

inline mat<4, 4>::mat(vec<4> const &v0, vec<4> const &v1, vec<4> const &v2,
                      vec<4> const &v3)
    : value{vec<4>(v0), vec<4>(v1), vec<4>(v2), vec<4>(v3)} {}

inline vec<4> &mat<4, 4>::operator[](int i) { return value[i]; }

inline vec<4> const &mat<4, 4>::operator[](int i) const { return value[i]; }

inline mat<4, 4> operator+(mat<4, 4> const &m1, mat<4, 4> const &m2) {
    return mat<4, 4>(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2],
                     m1[3] + m2[3]);
}

inline vec<4> operator*(const mat<4, 4> &m, const vec<4> &v) {
    const vec<4> Mov0(v[0]);
    const vec<4> Mov1(v[1]);
    const vec<4> Mul0 = m[0] * Mov0;
    const vec<4> Mul1 = m[1] * Mov1;
    const vec<4> Add0 = Mul0 + Mul1;
    const vec<4> Mov2(v[2]);
    const vec<4> Mov3(v[3]);
    const vec<4> Mul2 = m[2] * Mov2;
    const vec<4> Mul3 = m[3] * Mov3;
    const vec<4> Add1 = Mul2 + Mul3;
    const vec<4> Add2 = Add0 + Add1;
    return Add2;
}

inline mat<4, 4> operator*(const mat<4, 4> &m1, const mat<4, 4> &m2) {
    const vec<4> SrcA0 = m1[0];
    const vec<4> SrcA1 = m1[1];
    const vec<4> SrcA2 = m1[2];
    const vec<4> SrcA3 = m1[3];

    const vec<4> SrcB0 = m2[0];
    const vec<4> SrcB1 = m2[1];
    const vec<4> SrcB2 = m2[2];
    const vec<4> SrcB3 = m2[3];

    mat<4, 4> Result;
    Result[0] = SrcA0 * SrcB0[0] + SrcA1 * SrcB0[1] + SrcA2 * SrcB0[2] +
                SrcA3 * SrcB0[3];
    Result[1] = SrcA0 * SrcB1[0] + SrcA1 * SrcB1[1] + SrcA2 * SrcB1[2] +
                SrcA3 * SrcB1[3];
    Result[2] = SrcA0 * SrcB2[0] + SrcA1 * SrcB2[1] + SrcA2 * SrcB2[2] +
                SrcA3 * SrcB2[3];
    Result[3] = SrcA0 * SrcB3[0] + SrcA1 * SrcB3[1] + SrcA2 * SrcB3[2] +
                SrcA3 * SrcB3[3];
    return Result;
}

inline mat<4, 4> perspective(float fovy, float aspect, float zNear,
                             float zFar) {
    assert(std::abs(aspect - std::numeric_limits<float>::epsilon()) >
           static_cast<float>(0));

    const float tanHalfFovy = std::tan(fovy / static_cast<float>(2));

    mat<4, 4> Result(static_cast<float>(0));
    Result[0][0] = static_cast<float>(1) / (aspect * tanHalfFovy);
    Result[1][1] = static_cast<float>(1) / (tanHalfFovy);
    Result[2][2] = -(zFar + zNear) / (zFar - zNear);
    Result[2][3] = -static_cast<float>(1);
    Result[3][2] = -(static_cast<float>(2) * zFar * zNear) / (zFar - zNear);
    return Result;
}

inline mat<4, 4> rotate(mat<4, 4> const &m, float angle, vec<3> const &v) {
    float const a = angle;
    float const c = cos(a);
    float const s = sin(a);

    vec<3> axis(normalize(v));
    vec<3> temp((float(1) - c) * axis);

    mat<4, 4> Rotate;
    Rotate[0][0] = c + temp[0] * axis[0];
    Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
    Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

    Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
    Rotate[1][1] = c + temp[1] * axis[1];
    Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

    Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
    Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
    Rotate[2][2] = c + temp[2] * axis[2];

    mat<4, 4> Result;
    Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
    Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
    Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
    Result[3] = m[3];
    return Result;
}

inline mat<4, 4> look_at(const vec<3> &eye, const vec<3> &center,
                         const vec<3> &up) {
    const vec<3> f(normalize(center - eye));
    const vec<3> s(normalize(cross(f, up)));
    const vec<3> u(cross(s, f));

    mat<4, 4> Result(1);
    Result[0][0] = s.x;
    Result[1][0] = s.y;
    Result[2][0] = s.z;
    Result[0][1] = u.x;
    Result[1][1] = u.y;
    Result[2][1] = u.z;
    Result[0][2] = -f.x;
    Result[1][2] = -f.y;
    Result[2][2] = -f.z;
    Result[3][0] = -dot(s, eye);
    Result[3][1] = -dot(u, eye);
    Result[3][2] = dot(f, eye);
    return Result;
}

#endif // GEOMETRY_MATRIX_HPP