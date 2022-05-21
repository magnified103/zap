#ifndef ENTITY_ANIMATION_HPP
#define ENTITY_ANIMATION_HPP

#include <vector>

#include "cereal/cereal.hpp"

struct animation {
    std::vector<int> frame_id;
    float frame_time;
    float remaining_frame_time;
    int initial_phase;
    int current_phase;

    void activate() {
        current_phase = initial_phase;
        remaining_frame_time = frame_time;
    }

    void update_frame_id(float delta_time) {
        remaining_frame_time -= delta_time;
        if (remaining_frame_time <= 0) {
            remaining_frame_time = frame_time;
            current_phase++;
            if (current_phase == static_cast<int>(frame_id.size())) {
                current_phase = 0;
            }
        }
    }

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(frame_id), CEREAL_NVP(frame_time), CEREAL_NVP(remaining_frame_time),
                CEREAL_NVP(initial_phase));
    }
};

#endif // ENTITY_ANIMATION_HPP