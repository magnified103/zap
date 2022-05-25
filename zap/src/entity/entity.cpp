#include <algorithm>
#include <cmath>

#include "entity/entity.hpp"
#include "entity/inventory.hpp"
#include "geometry/vector.hpp"

entity_movement entity::try_move(vec2 direction, float speed, float delta_time) {
    float v0 = current_movement.current_vertical_speed;
    float v = v0 + g_force * delta_time;
    float s0 = current_movement.position.y;
    float s = s0 + v0 * delta_time + 0.5f * g_force * delta_time * delta_time;
    entity_movement next_movement;
    next_movement.current_vertical_speed = v;
    next_movement.position.y = s;
    if (speed && dot(direction, direction) > 0) {
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
entity_movement entity::try_move_without_gravity(vec2 direction, float speed, float delta_time) {
    entity_movement next_movement = current_movement;
    if (speed && dot(direction, direction) > 0) {
        direction = normalize(direction);
        vec2 h_position{current_movement.position.x, current_movement.position.z};
        h_position = h_position + direction * speed;
        next_movement.position.x = h_position.x;
        next_movement.position.z = h_position.y;
    }
    return next_movement;
}

float entity::calculate_distance(const vec3 &position) const {
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

bool entity::collide(const vec3 &position) const {
    return calculate_distance(position) <= hitbox_radius;
}

bool entity::collide(const vec3 &prev, const vec3 &next) const {
    const vec3 dir = next - prev;
    float lo = 0.0f, hi = 1.0f;

    // ternary search
    constexpr int ITERATIONS = 10;
    for (int iter = ITERATIONS; iter; iter--) {
        float x = lo + (hi - lo) / 3.0f;
        float y = hi - (hi - lo) / 3.0f;
        if (calculate_distance(prev + dir * x) < calculate_distance(prev + dir * y)) {
            hi = y;
        } else {
            lo = x;
        }
    }
    return collide(prev + dir * lo);
}