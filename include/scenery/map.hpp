#ifndef SCENERY_MAP_HPP
#define SCENERY_MAP_HPP

#include <istream>
#include <vector>

class map_grid {
public:
    map_grid(std::istream &&in);
    bool collide(int x, int y);

private:
    int width;
    int height;
    int grid_size;
    std::vector<std::vector<int>> grid;
};

#endif // SCENERY_MAP_HPP