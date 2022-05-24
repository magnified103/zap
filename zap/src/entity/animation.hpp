#ifndef ENTITY_ANIMATION_HPP
#define ENTITY_ANIMATION_HPP

#include <vector>

#include <cereal/cereal.hpp>

#include "timer/delay.hpp"

struct animation {
    std::vector<int> frame_id;
    delay frame;
    int initial_phase;
    int current_phase;

    void activate() {
        current_phase = initial_phase;
        frame.activate();
    }

    void update_frame_id(float delta_time) {
        frame.update(delta_time);
        if (frame.ended()) {
            frame.activate();
            current_phase++;
            if (current_phase == static_cast<int>(frame_id.size())) {
                current_phase = 0;
            }
        }
    }

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(frame_id), CEREAL_NVP(frame), CEREAL_NVP(initial_phase));
    }
};

#endif // ENTITY_ANIMATION_HPP