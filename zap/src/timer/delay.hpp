#ifndef TIMER_DELAY_HPP
#define TIMER_DELAY_HPP

#include <cereal/cereal.hpp>

struct delay {
    float delay;
    float remaining_delay;

    void activate() { remaining_delay = delay; }

    void update(float delta_time) { remaining_delay -= delta_time; }

    bool ended() { return remaining_delay <= 0; }

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(delay), CEREAL_NVP(remaining_delay));
    }
};

#endif // TIMER_DELAY_HPP