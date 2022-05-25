#include <vector>

#include "entity/entity.hpp"
#include "entity/player3d.hpp"
#include "geometry/matrix.hpp"
#include "geometry/trigonometric.hpp"
#include "geometry/vector.hpp"
#include "opengl/gl.hpp"
#include "scenery/camera3d.hpp"

struct map3d;

void player3d::render_view(const map3d &map, const std::vector<GLuint> &surface_ids,
                           const float &screen_width, const float &screen_height,
                           const GLuint &shader_program) {
    camera.render(current_movement.position + camera_position, vec3{0, 1, 0}, map, surface_ids,
                  screen_width, screen_height, shader_program);
}

entity_movement player3d::try_move(float forward, float backward, float left, float right, int type,
                                   float delta_time) {
    const mat4 fw(1);
    const mat4 bw = rotate(fw, radians(180.0f), vec3{0, 1, 0});
    const mat4 lt = rotate(fw, radians(90.0f), vec3{0, 1, 0});
    const mat4 rt = rotate(fw, radians(-90.0f), vec3{0, 1, 0});

    mat4 rotation = forward * fw + backward * bw + left * lt + right * rt;
    vec4 view = rotation * vec4{camera.get_view_vector(), 1.0f};

    float speed = 0;
    if (type == 1) {
        // walk
        speed = walk_speed;
    } else if (type == 2) {
        // run
        speed = run_speed;
    }
    return entity::try_move(vec2{view.x, view.z}, speed, delta_time);
}

entity_movement player3d::try_move_without_gravity(float forward, float backward, float left,
                                                   float right, int type, float delta_time) {
    const mat4 fw(1);
    const mat4 bw = rotate(fw, radians(180.0f), vec3{0, 1, 0});
    const mat4 lt = rotate(fw, radians(90.0f), vec3{0, 1, 0});
    const mat4 rt = rotate(fw, radians(-90.0f), vec3{0, 1, 0});

    mat4 rotation = forward * fw + backward * bw + left * lt + right * rt;
    vec4 view = rotation * vec4{camera.get_view_vector(), 1.0f};

    float speed = 0;
    if (type == 1) {
        // walk
        speed = walk_speed;
    } else if (type == 2) {
        // run
        speed = run_speed;
    }
    return entity::try_move_without_gravity(vec2{view.x, view.z}, speed, delta_time);
}

void player3d::trigger_jump() { current_movement.current_vertical_speed = jump_speed; }
