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

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(reload), CEREAL_NVP(highlighted_texture_index),
                CEREAL_NVP(regular_texture_index));
    }
    virtual ~inventory_item() = default;
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

    int get_selected_count() {
        if (selection < 0 || selection >= static_cast<int>(items.size())) {
            return 0;
        }
        return items[selection]->get_count();
    }

    void update_state(float delta_time) {
        for (auto &item : items) {
            item->reload.update(delta_time);
        }
    }

    void update_lock() {
        locked = false;
        for (auto &item : items) {
            if (!item->reload.ended()) {
                locked = true;
                break;
            }
        }
    }
};

#endif // ENTITY_INVENTORY_HPP