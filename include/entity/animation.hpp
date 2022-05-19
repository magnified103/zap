#ifndef ENTITY_ANIMATION_HPP
#define ENTITY_ANIMATION_HPP

#include <vector>

struct animation {
    std::vector<int> frame_id;
    float frame_time;
    float remaining_frame_time;
    int phase;
};

#endif // ENTITY_ANIMATION_HPP