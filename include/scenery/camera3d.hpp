#ifndef SCENERY_CAMERA3D_HPP
#define SCENERY_CAMERA3D_HPP

#include <cmath>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "cereal/cereal.hpp"
#include "geometry/vector.hpp"
#include "opengl/gl.hpp"
#include "scenery/map3d.hpp"

struct camera3d {
    float fov;
    float near;
    float far;
    float vertical_angle;
    float horizontal_angle;

    vec3 get_view_vector();

    void rotate(float horizontal_delta, float vertical_delta);

    void render(const vec3 &eye, const vec3 &up, const map3d &map,
                const std::vector<GLuint> &surface_ids,
                const float &screen_width, const float &screen_height,
                const GLuint &shader_program);

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(fov), CEREAL_NVP(near), CEREAL_NVP(far),
                CEREAL_NVP(vertical_angle), CEREAL_NVP(horizontal_angle));
    }
};

#endif // SCENERY_CAMERA3D_HPP