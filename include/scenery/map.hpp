#ifndef SCENERY_MAP_HPP
#define SCENERY_MAP_HPP

#include <istream>
#include <utility>
#include <vector>

#include "../wrapper/SDL.hpp"

class map_grid {
public:
    map_grid() = delete;
    map_grid(std::istream &in);
    map_grid(std::istream &&in);
    bool collide(int x, int y);

    sdl::surface get_surface() const;

    int get_value(int i, int j) const;

    int get_width() const;

    int get_height() const;

    int get_grid_size() const;

    std::pair<int, int> get_starting_position() const;

private:
    int width;
    int height;
    int grid_size;
    std::vector<std::vector<int>> grid;
};

#endif // SCENERY_MAP_HPP