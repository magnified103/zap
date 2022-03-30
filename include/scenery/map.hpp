#ifndef SCENERY_MAP_HPP
#define SCENERY_MAP_HPP

#include <istream>
#include <vector>

#include "../wrapper/SDL.hpp"

class map_grid {
public:
    map_grid() = delete;
    map_grid(std::istream &in);
    map_grid(std::istream &&in);
    bool collide(int x, int y);

    sdl::surface get_surface() const;

    int get_value(int i, int j) {
        if (i < 0 || i >= height || j < 0 || j >= width) {
            throw sdl::exception("Out of bound");
        }
        return grid[i][j];
    }

    int get_width() const { return width; }

    int get_height() const { return height; }

    int get_grid_size() const { return grid_size; }

private:
    int width;
    int height;
    int grid_size;
    std::vector<std::vector<int>> grid;
};

#endif // SCENERY_MAP_HPP