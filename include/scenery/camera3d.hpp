#ifndef SCENERY_CAMERA3D_HPP
#define SCENERY_CAMERA3D_HPP

#include <cmath>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "geometry/vector.hpp"
#include "map3d.hpp"
#include "opengl/gl.hpp"

struct camera3d {
    float fov;
    float near;
    float far;
    float vertical_angle;
    float horizontal_angle;

    void rotate(float horizontal_delta, float vertical_delta);

    void render(vec3 eye, vec3 up, const map3d &map,
                const std::vector<GLuint> surface_ids, float screen_width,
                float screen_height, GLuint shader_program);
};

#endif // SCENERY_CAMERA3D_HPP