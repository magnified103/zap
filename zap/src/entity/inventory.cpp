#include <memory>
#include <stdexcept>
#include <vector>

#include "entity/inventory.hpp"
#include "timer/delay.hpp"

void inventory::trigger_selection(map3d &map, player3d &player) {
    if (selection < 0 || selection >= static_cast<int>(items.size())) {
        throw std::runtime_error("Selection overflow");
    }
    items[selection]->use(map, player);
}

void inventory::trigger_selection(map3d &map, monster &monster, player3d &player) {
    if (selection < 0 || selection >= static_cast<int>(items.size())) {
        throw std::runtime_error("Selection overflow");
    }
    items[selection]->use(map, monster, player);
}

int inventory::get_selected_count() {
    if (selection < 0 || selection >= static_cast<int>(items.size())) {
        return 0;
    }
    return items[selection]->get_count();
}

void inventory::update_state(float delta_time) {
    for (auto &item : items) {
        item->reload.update(delta_time);
    }
}

void inventory::update_lock() {
    locked = false;
    for (auto &item : items) {
        if (!item->reload.ended()) {
            locked = true;
            break;
        }
    }
}