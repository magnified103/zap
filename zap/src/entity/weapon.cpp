#include <stdexcept>

#include "entity/inventory.hpp"
#include "entity/player3d.hpp"
#include "entity/projectile.hpp"
#include "entity/weapon.hpp"
#include "geometry/vector.hpp"
#include "scenery/map3d.hpp"

void melee_weapon::use(map3d &map, player3d &player) {
    auto position = player.current_movement.position + player.hand_position;
    auto direction = player.camera.get_view_vector();
    direction = normalize(direction);
    const auto target_position = position + direction;
    for (auto &monster : map.monsters) {
        if (monster.collide(target_position)) {
            monster.hitpoint -= melee_damage;
        }
    }
}

vec3 calculate_hand(const vec3 &relative_hand, const vec3 &direction) {
    vec2 hand_xz{relative_hand.x, relative_hand.z};
    float hand_len = std::sqrt(dot(hand_xz, hand_xz));
    hand_xz = vec2{direction.x, direction.z};
    float direction_len = std::sqrt(dot(hand_xz, hand_xz));
    if (direction_len) {
        hand_xz = hand_xz * (hand_len / direction_len);
    }
    return vec3{hand_xz.x, relative_hand.y, hand_xz.y};
}

void ranged_weapon::use(map3d &map, player3d &player) {
    if (projectile_type < 0 || projectile_type >= static_cast<int>(map.projectile_types.size())) {
        throw std::runtime_error("Invalid projectile type");
    }
    if (!activated || number_of_rounds <= 0 || !reload.ended()) {
        return;
    }
    reload.activate();
    number_of_rounds--;
    map.projectiles.push_back({map.projectile_types[projectile_type]});
    const auto direction = player.camera.get_view_vector();
    map.projectiles.back().ignite(player.current_movement.position +
                                      calculate_hand(player.hand_position, direction),
                                  direction);
}

void ranged_weapon::use(map3d &map, monster &monster, player3d &player) {
    if (projectile_type < 0 || projectile_type >= static_cast<int>(map.projectile_types.size())) {
        throw std::runtime_error("Invalid projectile type");
    }
    if (!activated || number_of_rounds <= 0 || !reload.ended()) {
        return;
    }
    reload.activate();
    number_of_rounds--;
    map.projectiles.push_back({map.projectile_types[projectile_type]});
    const auto direction = player.current_movement.position - monster.current_movement.position;
    map.projectiles.back().ignite(monster.current_movement.position +
                                      calculate_hand(monster.hand_position, direction),
                                  direction);
}