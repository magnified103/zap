#ifndef ENTITY_INVENTORY_HPP
#define ENTITY_INVENTORY_HPP

#include <memory>
#include <stdexcept>
#include <vector>

#include <cereal/cereal.hpp>

#include "timer/delay.hpp"

struct map3d;
struct player3d;
struct monster;

struct inventory_item {
    delay reload;
    int highlighted_texture_index;
    int regular_texture_index;

    virtual void use(map3d &map, player3d &player) = 0;

    virtual void use(map3d &map, monster &monster, player3d &player) = 0;

    virtual int get_count() { return 0; }

    virtual ~inventory_item() = default;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(reload), CEREAL_NVP(highlighted_texture_index),
                CEREAL_NVP(regular_texture_index));
    }
};

struct inventory {
    std::vector<std::unique_ptr<inventory_item>> items;
    int selection;
    bool locked;

    void trigger_selection(map3d &map, player3d &player);

    void trigger_selection(map3d &map, monster &monster, player3d &player);

    int get_selected_count();

    void update_state(float delta_time);

    void update_lock();

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(items), CEREAL_NVP(selection), CEREAL_NVP(locked));
    }
};

#endif // ENTITY_INVENTORY_HPP