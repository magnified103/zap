#ifndef ENTITY_ENTITY_HPP
#define ENTITY_ENTITY_HPP

#include "cereal/cereal.hpp"
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
    static constexpr float g_force = -10.0f; // m/s^2
    float walk_speed;                        // m/s
    float run_speed;                         // m/s
    float jump_speed;                        // m/s
    entity_movement current_movement;
    inventory current_inventory;
    vec3 hand_position;

    float hitbox_height;
    float hitbox_radius; // the hitbox looks like a big fucking cylinder hah
    int hitpoint;

    entity_movement try_move(vec2 direction, float speed, float delta_time) {
        float v0 = current_movement.current_vertical_speed;
        float v = v0 + g_force * delta_time;
        float s0 = current_movement.position.y;
        float s = s0 + v0 * delta_time + 0.5f * g_force * delta_time * delta_time;
        entity_movement next_movement;
        next_movement.current_vertical_speed = v;
        next_movement.position.y = s;
        if (speed) {
            direction = normalize(direction);
            vec2 h_position{current_movement.position.x, current_movement.position.z};
            h_position = h_position + direction * speed;
            next_movement.position.x = h_position.x;
            next_movement.position.z = h_position.y;
        } else {
            next_movement.position.x = current_movement.position.x;
            next_movement.position.z = current_movement.position.z;
        }
        return next_movement;
    }
    entity_movement try_move_without_gravity(vec2 direction, float speed, float delta_time) {
        entity_movement next_movement = current_movement;
        if (speed) {
            direction = normalize(direction);
            vec2 h_position{current_movement.position.x, current_movement.position.z};
            h_position = h_position + direction * speed;
            next_movement.position.x = h_position.x;
            next_movement.position.z = h_position.y;
        }
        return next_movement;
    }

    float calculate_distance(const vec3 &position) const {
        if (position.y > current_movement.position.y + hitbox_height ||
            position.y < current_movement.position.y) {
            const vec3 distance_bottom = position - current_movement.position;
            const vec3 distance_top =
                position - (current_movement.position + vec3{0, hitbox_height, 0});
            return std::sqrt(
                std::min(dot(distance_bottom, distance_bottom), dot(distance_top, distance_top)));
        }
        const vec2 hitbox_center{current_movement.position.x, current_movement.position.z};
        const vec2 point{position.x, position.z};
        return std::sqrt(dot(point - hitbox_center, point - hitbox_center));
    }

    bool collide(const vec3 &position) const {
        return calculate_distance(position) <= hitbox_radius;
    }

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(walk_speed), CEREAL_NVP(run_speed), CEREAL_NVP(jump_speed),
                CEREAL_NVP(current_movement), CEREAL_NVP(current_inventory),
                CEREAL_NVP(hand_position), CEREAL_NVP(hitbox_height), CEREAL_NVP(hitbox_radius),
                CEREAL_NVP(hitpoint));
    }
};

#endif // ENTITY_ENTITY_HPP