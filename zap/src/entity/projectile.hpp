#ifndef ENTITY_PROJECTILE_HPP
#define ENTITY_PROJECTILE_HPP

#include <cereal/cereal.hpp>

#include "geometry/vector.hpp"

struct projectile_type {
    float speed;
    int move_id;
    float move_size;
    int end_id;
    float end_size;
    float end_time;
    float blast_radius;
    int damage;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(speed), CEREAL_NVP(move_id), CEREAL_NVP(move_size), CEREAL_NVP(end_id),
                CEREAL_NVP(end_size), CEREAL_NVP(end_time), CEREAL_NVP(blast_radius),
                CEREAL_NVP(damage));
    }
};

struct projectile {
    projectile_type base;
    vec3 velocity; // anti-gravity effect lol
    vec3 position;
    int phase;
    float end_time;

    void ignite(vec3 initial_position, vec3 direction);

    vec3 try_move(float delta_time);

    void explode(float delta_time);
};

#endif // ENTITY_PROJECTILE_HPP