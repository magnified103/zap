#include <algorithm>
#include <istream>
#include <optional>
#include <utility>
#include <vector>

#include "../../include/geometry/segment.hpp"
#include "../../include/geometry/vector.hpp"
#include "../../include/scenery/map.hpp"
#include "../../include/scenery/wall.hpp"
#include "../../include/wrapper/SDL.hpp"

map_grid::map_grid(std::istream &in) {
    in >> height >> width >> grid_size;
    if (width <= 0 || height <= 0 || grid_size <= 0) {
        throw sdl::exception("Grid sizes must be positive integers");
    }
    grid.assign(height, std::vector<int>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (!(in >> grid[i][j])) {
                throw sdl::exception("Bad grid data");
            }
        }
    }
}

map_grid::map_grid(std::istream &&in) : map_grid(in) {}

bool map_grid::collide(int x, int y) {
    if (x < 0 || x >= width * grid_size || y < 0 || y >= height * grid_size) {
        return true;
    }
    return grid[y / grid_size][x / grid_size] == 1;
}

sdl::surface map_grid::get_surface() const {
    sdl::surface surface{width * grid_size, height * grid_size};
    surface.fill(sdl::color{0x00, 0x00, 0x00});
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == 1) {
                surface.fill_rect(sdl::rect{j * grid_size, i * grid_size,
                                            grid_size, grid_size},
                                  sdl::color{0xff, 0xff, 0xff});
            }
        }
    }
    return surface;
}

int map_grid::get_value(int x, int y) const {
    if (y < 0 || y >= height || x < 0 || x >= width) {
        throw sdl::exception("Out of bound");
    }
    return grid[y][x];
}

int map_grid::get_width() const { return width; }

int map_grid::get_height() const { return height; }

int map_grid::get_grid_size() const { return grid_size; }

std::pair<int, int> map_grid::get_starting_position() const {
    std::vector<std::pair<int, int>> candidates;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == -1) {
                candidates.emplace_back(j, i);
            }
        }
    }
    if (candidates.size() != 1) {
        throw sdl::exception("Wrong number of starting potisions");
    }
    return candidates[0];
}

std::optional<std::pair<point2d<float>, sdl::color>>
map_grid::ray_cast(ray<float> ray) const {
    std::optional<std::pair<point2d<float>, sdl::color>> best_intersection;
    float minimum_distance = 0;
    sdl::color color_parallel_x = sdl::colors::aqua;
    sdl::color color_parallel_y = sdl::colors::aquamarine;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] == 1) {
                point2d<float> a{static_cast<float>(x * grid_size),
                                 static_cast<float>(y * grid_size)};
                point2d<float> b{static_cast<float>((x + 1) * grid_size),
                                 static_cast<float>(y * grid_size)};
                point2d<float> c{static_cast<float>(x * grid_size),
                                 static_cast<float>((y + 1) * grid_size)};
                point2d<float> d{static_cast<float>((x + 1) * grid_size),
                                 static_cast<float>((y + 1) * grid_size)};
                auto intersection = get_intersection(ray, {a, b});
                if (intersection) {
                    float distance = (*intersection - ray.a).abs();
                    if (!best_intersection || minimum_distance > distance) {
                        best_intersection = {*intersection, color_parallel_x};
                        minimum_distance = distance;
                    }
                }
                intersection = get_intersection(ray, {a, c});
                if (intersection) {
                    float distance = (*intersection - ray.a).abs();
                    if (!best_intersection || minimum_distance > distance) {
                        best_intersection = {*intersection, color_parallel_y};
                        minimum_distance = distance;
                    }
                }
                intersection = get_intersection(ray, {b, d});
                if (intersection) {
                    float distance = (*intersection - ray.a).abs();
                    if (!best_intersection || minimum_distance > distance) {
                        best_intersection = {*intersection, color_parallel_y};
                        minimum_distance = distance;
                    }
                }
                intersection = get_intersection(ray, {c, d});
                if (intersection) {
                    float distance = (*intersection - ray.a).abs();
                    if (!best_intersection || minimum_distance > distance) {
                        best_intersection = {*intersection, color_parallel_x};
                        minimum_distance = distance;
                    }
                }
            }
        }
    }
    point2d<float> a{0, 0};
    point2d<float> b{static_cast<float>(width * grid_size), 0};
    point2d<float> c{0, static_cast<float>(height * grid_size)};
    point2d<float> d{static_cast<float>(width * grid_size),
                     static_cast<float>(height * grid_size)};
    auto intersection = get_intersection(ray, {a, b});
    if (intersection) {
        float distance = (*intersection - ray.a).abs();
        if (!best_intersection || minimum_distance > distance) {
            best_intersection = {*intersection, color_parallel_x};
            minimum_distance = distance;
        }
    }
    intersection = get_intersection(ray, {a, c});
    if (intersection) {
        float distance = (*intersection - ray.a).abs();
        if (!best_intersection || minimum_distance > distance) {
            best_intersection = {*intersection, color_parallel_y};
            minimum_distance = distance;
        }
    }
    intersection = get_intersection(ray, {b, d});
    if (intersection) {
        float distance = (*intersection - ray.a).abs();
        if (!best_intersection || minimum_distance > distance) {
            best_intersection = {*intersection, color_parallel_y};
            minimum_distance = distance;
        }
    }
    intersection = get_intersection(ray, {c, d});
    if (intersection) {
        float distance = (*intersection - ray.a).abs();
        if (!best_intersection || minimum_distance > distance) {
            best_intersection = {*intersection, color_parallel_x};
            minimum_distance = distance;
        }
    }
    return best_intersection;
}

std::pair<float, sdl::color> map_grid::hit_distance(point2d<float> source,
                                                    ray<float> ray) const {
    auto intersection = ray_cast(ray);
    if (intersection) {
        float distance = (intersection->first - source).abs();
        if (distance <= render_distance) {
            return {distance, intersection->second};
        }
    }
    return {infinity_distance, sdl::colors::black};
}

std::vector<wall> map_grid::get_walls() const {
    std::vector<wall> walls;
    sdl::color color_parallel_x = sdl::colors::aqua;
    sdl::color color_parallel_z = sdl::colors::aquamarine;
    float cell_size = grid_size;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == 1) {
                walls.push_back({{{j * cell_size, i * cell_size},
                                  {(j + 1) * cell_size, i * cell_size}},
                                 color_parallel_x});
                walls.push_back({{{j * cell_size, i * cell_size},
                                  {j * cell_size, (i + 1) * cell_size}},
                                 color_parallel_z});
                walls.push_back({{{(j + 1) * cell_size, i * cell_size},
                                  {(j + 1) * cell_size, (i + 1) * cell_size}},
                                 color_parallel_z});
                walls.push_back({{{j * cell_size, (i + 1) * cell_size},
                                  {(j + 1) * cell_size, (i + 1) * cell_size}},
                                 color_parallel_x});
            }
        }
    }
    return walls;
}