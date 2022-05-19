#ifndef ENTITY_INVENTORY_HPP
#define ENTITY_INVENTORY_HPP

#include <memory>
#include <stdexcept>
#include <vector>

#include "cereal/cereal.hpp"

struct map3d;
struct player3d;
struct monster;

struct inventory_item {
    float reload_time;
    float remaining_reload_time;

    virtual void use(map3d &map, player3d &player) = 0;

    virtual void use(map3d &map, monster &monster, player3d &player) = 0;

    virtual void update_state(float delta_time) { remaining_reload_time -= delta_time; }

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(reload_time), CEREAL_NVP(remaining_reload_time));
    }
};

struct inventory {
    std::vector<std::unique_ptr<inventory_item>> items;
    int selection;
    bool locked;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(items), CEREAL_NVP(selection), CEREAL_NVP(locked));
    }

    void trigger_selection(map3d &map, player3d &player) {
        if (selection < 0 || selection >= static_cast<int>(items.size())) {
            throw std::runtime_error("Selection overflow");
        }
        items[selection]->use(map, player);
    }

    void trigger_selection(map3d &map, monster &monster, player3d &player) {
        if (selection < 0 || selection >= static_cast<int>(items.size())) {
            throw std::runtime_error("Selection overflow");
        }
        items[selection]->use(map, monster, player);
    }

    void update_state(float delta_time) {
        for (auto &item : items) {
            item->update_state(delta_time);
        }
    }
};

#endif // ENTITY_INVENTORY_HPP