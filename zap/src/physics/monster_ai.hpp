#ifndef PHYSICS_MONSTER_AI_HPP
#define PHYSICS_MONSTER_AI_HPP

#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>

#include "entity/player3d.hpp"
#include "geometry/vector.hpp"
#include "scenery/map3d.hpp"

struct monster_ai {
    virtual std::vector<vec2> suggest(map3d &map, player3d &player) = 0;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {}
};

struct monster_ai_pathfinding : monster_ai {
    std::vector<vec2> suggest(map3d &map, player3d &player) override {
        int column_index = std::floor(player.current_movement.position.x / map.cell_size);
        int row_index = std::floor(player.current_movement.position.z / map.cell_size);
        int number_of_columns = map.number_of_columns;
        int number_of_rows = map.number_of_rows;

        // out of bound?
        if (column_index < 0 || column_index >= number_of_columns || row_index < 0 ||
            row_index >= number_of_rows) {
            throw std::runtime_error("Out of bound");
        }
        std::vector<std::vector<int>> dist(number_of_columns, std::vector<int>(number_of_rows, -1));
        dist[column_index][row_index] = 0;

        std::queue<std::pair<int, int>> q;
        q.emplace(column_index, row_index);

        while (!q.empty()) {
            auto [current_column, current_row] = q.front();
            q.pop();

            constexpr std::pair<int, int> adjacent_changes[] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

            for (auto [d_column, d_row] : adjacent_changes) {
                int next_column = current_column + d_column;
                int next_row = current_row + d_row;

                // out of bound?
                if (next_column < 0 || next_column >= number_of_columns || next_row < 0 ||
                    next_row >= number_of_rows) {
                    continue;
                }
                if (dist[next_column][next_row] == -1 &&
                    map.terrain[current_column][current_row].floor_level <=
                        map.terrain[next_column][next_row].floor_level + 0.2f) {
                    dist[next_column][next_row] = dist[current_column][current_row] + 1;
                    q.emplace(next_column, next_row);
                }
            }
        }

        std::vector<vec2> suggestions;
        for (auto &monster : map.monsters) {
            if (!monster.shooting_time.ended()) {
                suggestions.push_back({0.0f, 0.0f});
                continue;
            }
            int monster_column_index =
                std::floor(monster.current_movement.position.x / map.cell_size);
            int monster_row_index = std::floor(monster.current_movement.position.z / map.cell_size);

            if (dist[monster_column_index][monster_row_index] == -1) {
                suggestions.push_back({0, 0});
                continue;
            }

            if (dist[monster_column_index][monster_row_index] == 0) {
                // same region
                const auto direction =
                    player.current_movement.position - monster.current_movement.position;
                suggestions.push_back({direction.x, direction.z});
            } else {
                constexpr std::pair<int, int> adjacent_changes[] = {
                    {-1, 0}, {0, -1}, {0, 1}, {1, 0}};

                bool found_path = false;

                for (auto [d_column, d_row] : adjacent_changes) {
                    int next_column = monster_column_index + d_column;
                    int next_row = monster_row_index + d_row;

                    // out of bound?
                    if (next_column < 0 || next_column >= number_of_columns || next_row < 0 ||
                        next_row >= number_of_rows) {
                        continue;
                    }
                    if (dist[next_column][next_row] + 1 ==
                            dist[monster_column_index][monster_row_index] &&
                        map.terrain[monster_column_index][monster_row_index].floor_level <=
                            map.terrain[next_column][next_row].floor_level + 0.2f) {
                        suggestions.push_back(
                            {static_cast<float>(d_column), static_cast<float>(d_row)});
                        found_path = true;
                        break;
                    }
                }

                if (!found_path) {
                    suggestions.push_back({0.0f, 0.0f});
                }
            }

            // party time
            monster.current_inventory.update_lock();
            if (!monster.current_inventory.locked) {
                monster.current_inventory.trigger_selection(map, monster, player);
                monster.animation_phase = 1;
                monster.shooting.activate();
                monster.shooting_time.activate();
            } else {
                monster.animation_phase = 0;
                monster.moving.activate();
            }
        }

        return suggestions;
    }

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(cereal::make_nvp("monster_ai", (cereal::base_class<monster_ai>(this))));
    }
};

CEREAL_REGISTER_TYPE(monster_ai_pathfinding);

#endif // PHYSICS_MONSTER_AI_HPP