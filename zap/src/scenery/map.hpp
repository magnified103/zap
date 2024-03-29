#ifndef SCENERY_MAP_HPP
#define SCENERY_MAP_HPP

#include <istream>
#include <utility>
#include <vector>

#include "geometry/point.hpp"
#include "geometry/segment.hpp"
#include "scenery/wall.hpp"
#include "sdl2/SDL.hpp"

inline constexpr float infinity_distance = 100000;
inline constexpr float render_distance = 100000;

class map_grid {
public:
    map_grid(std::istream &in);
    map_grid(std::istream &&in);
    bool collide(int x, int y);

    sdl::surface get_surface() const;

    int get_value(int i, int j) const;

    int get_width() const;

    int get_height() const;

    int get_grid_size() const;

    std::pair<int, int> get_starting_position() const;

    std::optional<std::pair<point2d<float>, sdl::color>>
    ray_cast(ray<float> ray) const;

    std::pair<float, sdl::color> hit_distance(point2d<float> source,
                                              ray<float> ray) const;

    std::vector<wall> get_walls() const;

private:
    int width;
    int height;
    int grid_size;
    std::vector<std::vector<int>> grid;
};

#endif // SCENERY_MAP_HPP