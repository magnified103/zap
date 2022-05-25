#include <cctype>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <readline/history.h>
#include <readline/readline.h>

#include "core/map_editor.hpp"
#include "entity/animation.hpp"
#include "entity/player3d.hpp"
#include "entity/weapon.hpp"
#include "geometry/vector.hpp"
#include "physics/physics.hpp"
#include "scenery/hud.hpp"
#include "scenery/map3d.hpp"
#include "scenery/map_util.hpp"
#include "timer/delay.hpp"

struct repl {

    int process(map3d &map, player3d &player, hud &display, physics &engine,
                const std::string &full_cmd) {
        std::istringstream ss(full_cmd);
        if (std::string cmd; ss >> cmd) {
            if (cmd == "eval") {
                std::string path;
                if (ss >> path) {
                    if (path.empty() || path[0] != '.') {
                        return 0;
                    }
                    auto it = get_identifier(path);
                    const std::string identifier(std::next(path.cbegin()), it);
                    std::vector<std::string> argument_list;
                    std::string arg;
                    while (ss >> arg) {
                        argument_list.push_back(arg);
                    }
                    if (identifier == "map") {
                        return eval(map, std::string(it, path.cend()), argument_list);
                    }
                    if (identifier == "player") {
                        return eval(player, std::string(it, path.cend()), argument_list);
                    }
                    // if (identifier == "physics") {
                    //     return eval(engine, std::string(it, path.cend()), argument_list);
                    // }
                }
            }
            if (cmd == "fill") {
                std::size_t col, row, x1, z1, x2, z2;
                if (ss >> col >> row >> x1 >> z1 >> x2 >> z2) {
                    if (std::max(x2, col) < map.number_of_columns &&
                        std::max(z2, row) < map.number_of_rows && x1 <= x2 && z1 <= z2) {
                        const auto cur = map.terrain[col][row];
                        for (int i = x1; i <= x2; i++) {
                            for (int j = z1; j <= z2; j++) {
                                map.terrain[i][j] = cur;
                            }
                        }
                        return 1;
                    }
                }
            }
            if (cmd == "export") {
                std::string path;
                if (ss >> path) {
                    save_map(path, map, player, display, engine);
                    return 1;
                }
            }
            if (cmd == "exit") {
                return -1;
            }
        }
        return 0;
    }

    bool is_identifier(const char &c) { return std::isalpha(c) || std::isdigit(c) || c == '_'; }
    std::string::const_iterator get_identifier(const std::string &path) {
        auto it = std::next(path.begin());
        while (it != path.end() && is_identifier(*it)) {
            it++;
        }
        return it;
    }
    template <class T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    bool eval(T &value, const std::string &path, const std::vector<std::string> &argument_list) {
        if (!path.empty()) {
            return false;
        }
        if (argument_list.empty()) {
            std::cout << value << std::endl;
            return true;
        }
        if (argument_list[0] == "=") {
            if (argument_list.size() != 2) {
                return false;
            }
            std::istringstream ss(argument_list[1]);
            if (T tmp; ss >> tmp) {
                value = tmp;
                return true;
            }
        }
        return false;
    }

    bool eval(std::string &str, const std::string &path,
              const std::vector<std::string> &argument_list) {
        if (!path.empty()) {
            return false;
        }
        if (argument_list.empty()) {
            std::cout << str << std::endl;
            return true;
        }
        if (argument_list[0] == "=") {
            if (argument_list.size() != 2) {
                return false;
            }
            const std::string tmp = argument_list[1];
            if (tmp.size() >= 2 && tmp[0] == '\"' && tmp.back() == '\"') {
                str.assign(std::next(tmp.begin()), std::prev(tmp.end()));
                return true;
            }
        }
        return false;
    }

    template <class T>
    bool eval(std::vector<T> &vec, const std::string &path,
              const std::vector<std::string> &argument_list) {
        if (path.empty()) {
            if (argument_list.empty()) {
                return false;
            }
            if (argument_list[0] == "insert" || argument_list[0] == "erase") {
                if (argument_list.size() != 2) {
                    return false;
                }
                std::istringstream ss(argument_list[1]);
                if (std::size_t pos; ss >> pos) {
                    if (argument_list[0] == "insert") {
                        if (pos > vec.size()) {
                            return false;
                        }
                        vec.insert(vec.begin() + pos, T{});
                    }
                    if (argument_list[0] == "erase") {
                        if (pos >= vec.size()) {
                            return false;
                        }
                        vec.erase(vec.begin() + pos);
                    }
                    return true;
                }
            }
            return false;
        }
        if (path[0] != '[') {
            return false;
        }
        auto it = get_identifier(path);
        std::istringstream ss(std::string(std::next(path.begin()), it));
        if (std::size_t pos; ss >> pos) {
            if (pos < vec.size() && it != path.cend() && *it == ']') {
                return eval(vec[pos], std::string(std::next(it), path.cend()), argument_list);
            }
        }
        return false;
    }

    template <class T, std::size_t N>
    bool eval(std::array<T, N> &arr, const std::string &path,
              const std::vector<std::string> &argument_list) {
        if (path.empty()) {
            return false;
        }
        if (path[0] != '[') {
            return false;
        }
        auto it = get_identifier(path);
        std::istringstream ss(std::string(std::next(path.begin()), it));
        if (std::size_t pos; ss >> pos) {
            if (pos < arr.size() && it != path.cend() && *it == ']') {
                return eval(arr[pos], std::string(std::next(it), path.cend()), argument_list);
            }
        }
        return false;
    }

    template <class T>
    bool eval(std::unique_ptr<T> &ptr, const std::string &path,
              const std::vector<std::string> &argument_list) {
        if (path.empty() || path[0] != '<') {
            return false;
        }
        auto it = get_identifier(path);
        if (it == path.cend() || *it != '>') {
            return false;
        }
        const std::string identifier(std::next(path.cbegin()), it);
#define EVAL_NEW_UPCAST(derived)                                                                   \
    if (identifier == #derived) {                                                                  \
        if constexpr (std::is_base_of_v<T, derived>) {                                             \
            if (!ptr) {                                                                            \
                ptr = std::make_unique<derived>();                                                 \
            }                                                                                      \
            return eval(dynamic_cast<derived &>(*ptr), std::string(std::next(it), path.cend()),    \
                        argument_list);                                                            \
        }                                                                                          \
    }
        // write derived classes here
        EVAL_NEW_UPCAST(melee_weapon);
        EVAL_NEW_UPCAST(ranged_weapon);
#undef EVAL_NEW_UPCAST
        return false;
    }

#define EVAL_CLASS_MEMBER(object, member)                                                          \
    if (identifier == #member) {                                                                   \
        return eval(object.member, std::string(it, path.cend()), argument_list);                   \
    }

#define EVAL_CLASS_MEMBER_NO_RETURN(object, member)                                                \
    identifier == #member ? eval(object.member, std::string(it, path.cend()), argument_list) : false

#define EVAL_BASE_CLASS(object, base)                                                              \
    if (identifier == #base) {                                                                     \
        return eval(static_cast<base &>(object), std::string(it, path.cend()), argument_list);     \
    }

#define EVAL_MEMBER_SYNTAX()                                                                       \
    if (path.empty() || path[0] != '.') {                                                          \
        return false;                                                                              \
    }                                                                                              \
    auto it = get_identifier(path);                                                                \
    const std::string identifier(std::next(path.cbegin()), it);                                    \
    std::cerr << identifier << std::endl;

    template <int L>
    bool eval(vec<L> &vec, const std::string &path, const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        if constexpr (L >= 1) {
            EVAL_CLASS_MEMBER(vec, x);
        }
        if constexpr (L >= 2) {
            EVAL_CLASS_MEMBER(vec, y);
        }
        if constexpr (L >= 3) {
            EVAL_CLASS_MEMBER(vec, z);
        }
        if constexpr (L >= 4) {
            EVAL_CLASS_MEMBER(vec, w);
        }
        return false;
    }

    bool eval(map3d &map, const std::string &path, const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        if (EVAL_CLASS_MEMBER_NO_RETURN(map, number_of_columns)) {
            map.terrain.resize(map.number_of_columns);
            return true;
        }
        if (EVAL_CLASS_MEMBER_NO_RETURN(map, number_of_rows)) {
            for (auto &col : map.terrain) {
                col.resize(map.number_of_rows);
            }
            return true;
        }
        EVAL_CLASS_MEMBER(map, cell_size);
        EVAL_CLASS_MEMBER(map, terrain);
        // EVAL_CLASS_MEMBER(map, blocks);
        EVAL_CLASS_MEMBER(map, monsters);
        EVAL_CLASS_MEMBER(map, projectile_types);
        EVAL_CLASS_MEMBER(map, meshes);
        EVAL_CLASS_MEMBER(map, texture_paths);
        return false;
    }

    bool eval(tile &tile, const std::string &path, const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_CLASS_MEMBER(tile, top_mesh_index);
        EVAL_CLASS_MEMBER(tile, middle_mesh_index);
        EVAL_CLASS_MEMBER(tile, bottom_mesh_index);
        EVAL_CLASS_MEMBER(tile, floor_level);
        EVAL_CLASS_MEMBER(tile, ceil_level);
        EVAL_CLASS_MEMBER(tile, lightning);
        return false;
    }

    bool eval(block &block, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_CLASS_MEMBER(block, front_id);
        EVAL_CLASS_MEMBER(block, left_id);
        EVAL_CLASS_MEMBER(block, back_id);
        EVAL_CLASS_MEMBER(block, right_id);
        EVAL_CLASS_MEMBER(block, top_id);
        EVAL_CLASS_MEMBER(block, bottom_id);
        EVAL_CLASS_MEMBER(block, height_top);
        EVAL_CLASS_MEMBER(block, height_bottom);
        return false;
    }

    bool eval(entity_movement &movement, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_CLASS_MEMBER(movement, current_vertical_speed);
        EVAL_CLASS_MEMBER(movement, position);
        return false;
    }

    bool eval(inventory_item &item, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        // EVAL_CLASS_MEMBER(item, reload_time);
        // EVAL_CLASS_MEMBER(item, remaining_reload_time);
        return false;
    }

    bool eval(melee_weapon &weapon, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_BASE_CLASS(weapon, inventory_item);
        EVAL_CLASS_MEMBER(weapon, melee_damage);
        return false;
    }

    bool eval(ranged_weapon &weapon, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_BASE_CLASS(weapon, inventory_item);
        EVAL_CLASS_MEMBER(weapon, projectile_type);
        EVAL_CLASS_MEMBER(weapon, activated);
        EVAL_CLASS_MEMBER(weapon, number_of_rounds);
        EVAL_CLASS_MEMBER(weapon, max_number_of_rounds);
        return false;
    }

    bool eval(inventory &inventory, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_CLASS_MEMBER(inventory, items);
        EVAL_CLASS_MEMBER(inventory, selection);
        EVAL_CLASS_MEMBER(inventory, locked);
        return false;
    }

    bool eval(entity &entity, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_CLASS_MEMBER(entity, g_force);
        EVAL_CLASS_MEMBER(entity, walk_speed);
        EVAL_CLASS_MEMBER(entity, run_speed);
        EVAL_CLASS_MEMBER(entity, jump_speed);
        EVAL_CLASS_MEMBER(entity, current_movement);
        EVAL_CLASS_MEMBER(entity, current_inventory);
        EVAL_CLASS_MEMBER(entity, hand_position);
        EVAL_CLASS_MEMBER(entity, hitbox_height);
        EVAL_CLASS_MEMBER(entity, hitbox_radius);
        EVAL_CLASS_MEMBER(entity, hitpoint);
        return false;
    }

    bool eval(monster &monster, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_BASE_CLASS(monster, entity);
        // EVAL_CLASS_MEMBER(monster, front_id);
        EVAL_CLASS_MEMBER(monster, moving);
        EVAL_CLASS_MEMBER(monster, shooting);
        EVAL_CLASS_MEMBER(monster, animation_phase);
        EVAL_CLASS_MEMBER(monster, shooting_time);
        return false;
    }

    bool eval(animation &animation, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_CLASS_MEMBER(animation, frame_id);
        EVAL_CLASS_MEMBER(animation, frame);
        EVAL_CLASS_MEMBER(animation, initial_phase);
        EVAL_CLASS_MEMBER(animation, current_phase);
        return false;
    }

    bool eval(delay &delay, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_CLASS_MEMBER(delay, delay);
        EVAL_CLASS_MEMBER(delay, remaining_delay);
        return false;
    }

    bool eval(projectile_type &type, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_CLASS_MEMBER(type, speed);
        EVAL_CLASS_MEMBER(type, move_id);
        EVAL_CLASS_MEMBER(type, move_size);
        EVAL_CLASS_MEMBER(type, end_id);
        EVAL_CLASS_MEMBER(type, end_size);
        EVAL_CLASS_MEMBER(type, end_time);
        EVAL_CLASS_MEMBER(type, blast_radius);
        EVAL_CLASS_MEMBER(type, damage);
        return false;
    }

    bool eval(projectile &projectile, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_CLASS_MEMBER(projectile, base);
        EVAL_CLASS_MEMBER(projectile, velocity);
        EVAL_CLASS_MEMBER(projectile, position);
        EVAL_CLASS_MEMBER(projectile, phase);
        EVAL_CLASS_MEMBER(projectile, end_time);
        return false;
    }

    // bool eval(texture &texture, const std::string &path,
    //           const std::vector<std::string> &argument_list) {
    //     EVAL_MEMBER_SYNTAX();
    //     EVAL_CLASS_MEMBER(texture, surface_id);
    //     EVAL_CLASS_MEMBER(texture, positions);
    //     EVAL_CLASS_MEMBER(texture, UVs);
    //     return false;
    // }

    bool eval(mesh &mesh, const std::string &path, const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_CLASS_MEMBER(mesh, vertices);
        EVAL_CLASS_MEMBER(mesh, faces);
        return false;
    }

    bool eval(triangle &triangle, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_CLASS_MEMBER(triangle, vertex_indices);
        EVAL_CLASS_MEMBER(triangle, UVs);
        EVAL_CLASS_MEMBER(triangle, texture_index);
        return false;
    }

    bool eval(player3d &player, const std::string &path,
              const std::vector<std::string> &argument_list) {
        EVAL_MEMBER_SYNTAX();
        EVAL_BASE_CLASS(player, entity);
        EVAL_CLASS_MEMBER(player, camera_position);
        return false;
    }

#undef EVAL_CLASS_MEMBER
#undef EVAL_CLASS_MEMBER_NO_RETURN
#undef EVAL_BASE_CLASS
#undef EVAL_MEMBER_SYNTAX
};

void map_editor(const std::string &path) {
    map3d map;
    player3d player;
    hud display;
    physics engine;

    load_map(path, map, player, display, engine);

    repl interpreter;
    int code;

    // auto complete
    rl_bind_key('\t', rl_complete);

    do {
        char *input = readline("> ");
        if (!input) {
            break;
        }
        add_history(input);
        std::string full_cmd(input);
        free(input);
        code = interpreter.process(map, player, display, engine, full_cmd);
        if (code == 0) {
            std::cout << "Error" << std::endl;
        } else if (code == 1) {
            std::cout << "Succeeded" << std::endl;
        }
    } while (code != -1);
}

// #include <iostream>
// #include <sstream>
// #include <string>

// #include "core/map_editor.hpp"
// #include "entity/player3d.hpp"
// #include "geometry/vector.hpp"
// #include "physics/physics.hpp"
// #include "scenery/map3d.hpp"
// #include "scenery/map_util.hpp"

// void map_editor(const std::string &path) {
//     map3d map;
//     player3d player;
//     physics engine;
//     load_map(path, map, player, engine);
//     while (1) {
//         std::cout << "Command: ";
//         std::string command;
//         std::getline(std::cin, command);
//         std::stringstream ss(command);
//         std::string action, param;
//         if (!(ss >> action)) {
//             continue;
//         }
//         if (action == "edit") {
//             if (!(ss >> param)) {
//                 continue;
//             }
//             if (param == "map") {
//                 if (!(ss >> param)) {
//                     continue;
//                 }
//                 if (param == "number_of_columns") {
//                     int value;
//                     if (!(ss >> value)) {
//                         continue;
//                     }
//                     map.number_of_columns = value;
//                     map.terrain.resize(value);
//                 } else if (param == "number_of_rows") {
//                     int value;
//                     if (!(ss >> value)) {
//                         continue;
//                     }
//                     map.number_of_rows = value;
//                     for (auto &col : map.terrain) {
//                         col.resize(value);
//                     }
//                 } else if (param == "monsters") {
//                     if (!(ss >> param)) {
//                         continue;
//                     }
//                     if (param == "insert") {
//                         map.monsters.push_back({});
//                     } else {
//                         int index = std::stoi(param);
//                         if (!(ss >> param)) {
//                             continue;
//                         }
//                         if (param == "entity") {
//                             if (!(ss >> param)) {
//                                 continue;
//                             }
//                             if (param == "walk_speed") {
//                                 float value;
//                                 if (!(ss >> value)) {
//                                     continue;
//                                 }
//                                 map.monsters[index].walk_speed = value;
//                             } else if (param == "run_speed") {
//                                 float value;
//                                 if (!(ss >> value)) {
//                                     continue;
//                                 }
//                                 map.monsters[index].run_speed = value;
//                             }
//                         }
//                     }
//                 }
//             }
//         } else if (action == "copy") {
//             if (!(ss >> param)) {
//                 continue;
//             }
//             if (param == "map") {
//                 if (!(ss >> param)) {
//                     continue;
//                 }
//                 if (param == "terrain") {
//                     int col, row;
//                     if (!(ss >> col >> row)) {
//                         continue;
//                     }
//                     const auto cur = map.terrain[col][row];
//                     int x1, z1, x2, z2;
//                     if (!(ss >> x1 >> z1 >> x2 >> z2)) {
//                         continue;
//                     }
//                     for (int i = x1; i <= x2; i++) {
//                         for (int j = z1; j <= z2; j++) {
//                             map.terrain[i][j] = cur;
//                             map.terrain[i][j].point_a =
//                                 vec3{i * map.cell_size, 0, j * map.cell_size};
//                             map.terrain[i][j].point_b =
//                                 map.terrain[i][j].point_a + cur.point_b - cur.point_a;
//                         }
//                     }
//                 }
//             }
//         } else if (action == "save") {
//             if (!(ss >> param)) {
//                 continue;
//             }
//             save_map(param, map, player, engine);
//         } else if (action == "exit") {
//             break;
//         }
//     }
// }