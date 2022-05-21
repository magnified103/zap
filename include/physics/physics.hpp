#ifndef PHYSICS_PHYSICS_HPP
#define PHYSICS_PHYSICS_HPP

#include <memory>
#include <stdexcept>

#include "cereal/cereal.hpp"
#include "entity/entity.hpp"
#include "physics/monster_ai.hpp"
#include "scenery/map3d.hpp"
#include "wrapper/logging.hpp"

struct physics {
    std::unique_ptr<monster_ai> ai;
    bool forward_state;
    bool backward_state;
    bool left_state;
    bool right_state;
    bool jump_state;
    bool sprint_state;

    void trigger_player_keyboard(map3d &map, player3d &player, bool forward, bool backward,
                                 bool left, bool right, bool jump, bool sprint) {
        forward_state = forward;
        backward_state = backward;
        left_state = left;
        right_state = right;
        jump_state = jump;
        sprint_state = sprint;
    }

    void resolve_player_physics(map3d &map, player3d &player, float delta_time) {
        int type = sprint_state ? 2 : 1;
        if (!(forward_state || backward_state || left_state || right_state)) {
            type = 0;
        }
        bool floating = !map.collide_floor(player.try_move(0, 0, 0, 0, 0, 0.1).position);
        auto movement = player.try_move(forward_state, backward_state, left_state, right_state,
                                        type, delta_time);
        // if (!floating && !map.collide_floor(movement.position, 0.2f)) {
        //     // stair stepping?
        //     player.current_movement = movement;
        // } else {
        //     if (map.collide_floor(movement.position)) {
        //         movement = player.try_move_without_gravity(
        //             forward_state, backward_state, left_state, right_state, type, delta_time);
        //     }
        //     if (map.collide_floor(movement.position)) {
        //         movement = player.try_move(0, 0, 0, 0, 0, delta_time);
        //     }
        //     if (!map.collide_floor(movement.position)) {
        //         player.current_movement = movement;
        //     }
        // }
        if (map.collide_floor(movement.position)) {
            movement = player.try_move_without_gravity(forward_state, backward_state, left_state,
                                                       right_state, type, delta_time);
        }
        if (map.collide_floor(movement.position)) {
            movement = player.try_move(0, 0, 0, 0, 0, delta_time);
        }
        if (!map.collide_floor(movement.position)) {
            player.current_movement = movement;
        }
        if (jump_state && !floating) {
            player.trigger_jump();
        }
    }

    void resolve_projectile_physics(map3d &map, player3d &player, float delta_time) {
        // handle projectiles
        for (auto &projectile : map.projectiles) {
            if (projectile.phase >= 2) {
                continue;
            }
            bool ended = false;
            bool newly_exploded = false;
            if (projectile.phase == 0) {
                auto next_position = projectile.try_move(delta_time);
                if (map.collide_floor(next_position) || map.collide_ceil(next_position)) {
                    sdl::log_info("Projectile collides with floor / ceil, phase = %d",
                                  projectile.phase);
                    newly_exploded = true;
                    ended = true;
                } else {
                    for (const auto &monster : map.monsters) {
                        if (monster.collide(next_position)) {
                            sdl::log_info("Projectile collides with monster, phase = %d",
                                          projectile.phase);
                            newly_exploded = true;
                            ended = true;
                            break;
                        }
                    }
                }
                projectile.position = projectile.try_move(delta_time);
            } else {
                ended = true;
            }
            if (ended) {
                projectile.explode(delta_time);
            }
            if (newly_exploded) {
                for (auto &monster : map.monsters) {
                    if (monster.calculate_distance(projectile.position) <=
                        projectile.base.blast_radius) {
                        monster.hitpoint -= projectile.base.damage;
                        sdl::log_info("Projectile dealt %d, distance = %.2f",
                                      projectile.base.damage,
                                      monster.calculate_distance(projectile.position));
                    }
                }
                if (player.calculate_distance(projectile.position) <=
                    projectile.base.blast_radius) {
                    player.hitpoint -= projectile.base.damage;
                    sdl::log_info("You got hit, damage = %d, distance = %.2f",
                                  projectile.base.damage,
                                  player.calculate_distance(projectile.position));
                }
            }
        }
    }

    void resolve_monster_physics(map3d &map, player3d &player, float delta_time) {
        auto suggestions = ai->suggest(map, player);

        if (suggestions.size() != map.monsters.size()) {
            throw std::runtime_error("Monster movements mismatch");
        }
        for (int i = 0; i < static_cast<int>(suggestions.size()); i++) {
            auto &monster = map.monsters[i];

            // floating?
            bool floating = !map.collide_floor(monster.try_move(vec2{0, 0}, 0, 0.1).position);

            auto movement = monster.try_move(suggestions[i], monster.walk_speed, delta_time);
            // if (!floating && !map.collide_floor(movement.position, 0.2f)) {
            //     // stair stepping?
            //     monster.current_movement = movement;
            // }
            if (map.collide_floor(movement.position)) {
                movement = monster.try_move_without_gravity(suggestions[i], monster.walk_speed,
                                                            delta_time);
            }
            if (map.collide_floor(movement.position)) {
                movement = monster.try_move(vec2{0, 0}, 0, delta_time);
            }
            if (!map.collide_floor(movement.position)) {
                monster.current_movement = movement;
            }
            monster.update_animation(delta_time);
        }
    }

    void resolve(map3d &map, player3d &player, float delta_time) {
        resolve_player_physics(map, player, delta_time);
        resolve_projectile_physics(map, player, delta_time);
        resolve_monster_physics(map, player, delta_time);

        // remove ended projectiles
        auto &projectiles = map.projectiles;
        for (auto it = projectiles.begin(); it != projectiles.end();) {
            if (it->phase >= 2) {
                it = projectiles.erase(it);
            } else {
                it++;
            }
        }

        // remove dead monsters
        auto &monsters = map.monsters;
        for (auto it = monsters.begin(); it != monsters.end();) {
            if (it->hitpoint <= 0) {
                it = monsters.erase(it);
            } else {
                it++;
            }
        }
    }

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(ai));
    }
};

#endif // CORE_PHYSICS_HPP