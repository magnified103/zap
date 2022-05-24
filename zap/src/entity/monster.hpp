#ifndef ENTITY_MONSTER_HPP
#define ENTITY_MONSTER_HPP

#include <cereal/cereal.hpp>

#include "entity/animation.hpp"
#include "entity/entity.hpp"
#include "timer/delay.hpp"

struct monster : entity {
    animation moving;
    animation shooting;
    int animation_phase;
    delay shooting_time;

    void update_animation(float delta_time) {
        if (animation_phase == 0) {
            moving.update_frame_id(delta_time);
        }
        if (animation_phase == 1) {
            shooting.update_frame_id(delta_time);
            shooting_time.update(delta_time);
        }
    }

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(cereal::make_nvp("entity", (cereal::base_class<entity>(this))), CEREAL_NVP(moving),
                CEREAL_NVP(shooting), CEREAL_NVP(animation_phase), CEREAL_NVP(shooting_time));
    }
};

#endif // ENTITY_MONSTER_HPP