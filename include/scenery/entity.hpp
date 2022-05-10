#ifndef SCENERY_ENTITY_HPP
#define SCENERY_ENTITY_HPP

#include "geometry/vector.hpp"

struct entity {
    float walk_speed;
    float run_speed;
    vec3 position; // from his foot?
};

#endif // SCENERY_ENTITY_HPP