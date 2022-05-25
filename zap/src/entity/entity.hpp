#ifndef ENTITY_ENTITY_HPP
#define ENTITY_ENTITY_HPP

#include <cereal/cereal.hpp>

#include "entity/inventory.hpp"
#include "geometry/vector.hpp"

struct entity_movement {
    float current_vertical_speed; // m/s
    vec3 position;                // from his foot?

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(current_vertical_speed), CEREAL_NVP(position));
    }
};

struct entity {
    float g_force;    // m/s^2
    float walk_speed; // m/s
    float run_speed;  // m/s
    float jump_speed; // m/s
    entity_movement current_movement;
    inventory current_inventory;
    vec3 hand_position;

    float hitbox_height;
    float hitbox_radius; // the hitbox looks like a big fucking cylinder hah
    int hitpoint;

    entity_movement try_move(vec2 direction, float speed, float delta_time);

    entity_movement try_move_without_gravity(vec2 direction, float speed, float delta_time);

    float calculate_distance(const vec3 &position) const;

    bool collide(const vec3 &position) const;

    bool collide(const vec3 &prev, const vec3 &next) const;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(g_force), CEREAL_NVP(walk_speed), CEREAL_NVP(run_speed),
                CEREAL_NVP(jump_speed), CEREAL_NVP(current_movement), CEREAL_NVP(current_inventory),
                CEREAL_NVP(hand_position), CEREAL_NVP(hitbox_height), CEREAL_NVP(hitbox_radius),
                CEREAL_NVP(hitpoint));
    }
};

#endif // ENTITY_ENTITY_HPP