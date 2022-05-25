#include "entity/projectile.hpp"
#include "geometry/vector.hpp"

void projectile::ignite(vec3 initial_position, vec3 direction) {
    // shoot!
    velocity = normalize(direction) * base.speed;
    position = initial_position;
    phase = 0;
}

vec3 projectile::try_move(float delta_time) { return position + velocity * delta_time; }

void projectile::explode(float delta_time) {
    if (phase == 0) {
        phase = 1;
        end_time = base.end_time;
    } else {
        if (end_time < 0) {
            phase = 2;
        } else {
            end_time -= delta_time;
        }
    }
}