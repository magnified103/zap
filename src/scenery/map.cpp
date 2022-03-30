#include <istream>
#include <vector>

#include "../../include/scenery/map.hpp"
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
        return false;
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