#ifndef ENTITY_PLAYER3D_HPP
#define ENTITY_PLAYER3D_HPP

#include <vector>

#include <cereal/cereal.hpp>

#include "entity/entity.hpp"
#include "geometry/matrix.hpp"
#include "geometry/trigonometric.hpp"
#include "geometry/vector.hpp"
#include "opengl/gl.hpp"
#include "scenery/camera3d.hpp"

struct map3d;

struct player3d : entity {
    vec3 camera_position;
    camera3d camera;

    // runtime specific
    bool dead = false;

    void render_view(const map3d &map, const std::vector<GLuint> &surface_ids,
                     const float &screen_width, const float &screen_height,
                     const GLuint &shader_program);

    entity_movement try_move(float forward, float backward, float left, float right, int type,
                             float delta_time);

    entity_movement try_move_without_gravity(float forward, float backward, float left, float right,
                                             int type, float delta_time);

    void trigger_jump();

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(cereal::make_nvp("entity", (cereal::base_class<entity>(this))),
                CEREAL_NVP(camera_position), CEREAL_NVP(camera));
    }
};

#endif // ENTITY_PLAYER3D_HPP