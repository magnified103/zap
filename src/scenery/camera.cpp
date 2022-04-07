#include <algorithm>
#include <cmath>

#include "../../include/geometry/matrix.hpp"
#include "../../include/geometry/vector.hpp"
#include "../../include/scenery/camera.hpp"
#include "../../include/scenery/map.hpp"
#include "../../include/scenery/player.hpp"
#include "../../include/wrapper/SDL.hpp"

camera::camera() : camera(std::acos(-1) / 3) {}

camera::camera(float fov) : fov(fov), horizontal_angle(0), vertical_angle(0) {}

void camera::rotate(float horizontal_changes, float vertical_changes) {
    horizontal_angle += horizontal_changes;
    vertical_angle += vertical_changes;
}

void camera::render(sdl::renderer_handle renderer, player &player,
                    map_grid &grid, float width, float height) {
    int ray_count = 100;
    float base_angle = horizontal_angle - fov / 2;
    float base_w = fov / ray_count;
    std::vector<std::pair<float, sdl::color>> hits;
    point2d<float> player_position = player.get_position();
    // sdl::log_info("%.2f %.2f", player_position.x, player_position.y);
    for (int i = 0; i < ray_count; i++) {
        point2d<float> delta{std::cos(base_angle), std::sin(base_angle)};
        auto hit = grid.hit_distance(
            player_position, {player_position, player_position + delta});
        hits.emplace_back(hit.first * std::cos(base_angle - horizontal_angle),
                          hit.second);
        // sdl::log_info("%.2f %.2f %.2f", base_w, delta.x, delta.y);
        base_angle += base_w;
    }
    render_hits(renderer, hits, width / ray_count, height / 2, 20000, 200);
}

void camera::render_hits(sdl::renderer_handle renderer,
                         std::vector<std::pair<float, sdl::color>> hits,
                         float delta, float horizon, float base_height,
                         float base_color) {
    float x = 0;
    for (auto hit : hits) {
        float wall_height = base_height / hit.first;
        float scale = 1.0f - std::min(1.0f, hit.first / base_color);
        Uint8 r = hit.second.red() * scale;
        Uint8 g = hit.second.green() * scale;
        Uint8 b = hit.second.blue() * scale;
        renderer.set_draw_color(sdl::color{r, g, b});
        renderer.fill_rect(
            sdl::frect{x, horizon - wall_height / 2, delta, wall_height});
        x += delta;
    }
}

void camera::render3d(sdl::renderer_handle renderer, player &player,
                      map_grid &grid, float width, float height) {

    // walls
    auto walls = grid.get_walls();

    // wall height
    float wall_height = grid.get_grid_size();

    // eye but 2d
    vec2 eye_2d;

    // api mismatch workaround
    {
        auto pos = player.get_position();
        eye_2d = vec2{pos.x, pos.y};
    }

    // eye
    vec3 eye_3d{eye_2d.x, wall_height * 0.5f, eye_2d.y};

    // center
    vec3 center =
        eye_3d + vec3{std::cos(horizontal_angle), std::tan(vertical_angle),
                      std::sin(horizontal_angle)};

    // aspect
    float aspect = width / height;

    // vertical field of view
    float fovy = 2 * std::atan(1.0f / aspect * std::tan(fov / 2));

    // projection matrix
    auto projection = perspective(fovy, aspect, 0.01f, 1000.0f);

    // view matrix
    auto view = look_at(eye_3d, center, vec3{0, 1, 0});

    // translation
    auto translation = projection * view;

    // painter's algorithm
    //  let's sort the walls by the distance from their centroids to eye
    std::sort(walls.begin(), walls.end(),
              [&eye_2d](const wall &a, const wall &b) {
                  const auto vec_a = a.get_centroid() - eye_2d;
                  const auto vec_b = b.get_centroid() - eye_2d;
                  return dot(vec_a, vec_a) > dot(vec_b, vec_b);
              });

    static bool debug = false;

    if (!debug) {
        sdl::log_info("player position: (%.2f %.2f)", eye_2d.x, eye_2d.y);
    }

    width /= 2;
    height /= 2;

    // then print them one by one from the furthest
    for (const auto &wall : walls) {
        // get coordinates
        vec4 a{wall.endpoints[0].x, wall_height, wall.endpoints[0].y, 1};
        vec4 b{wall.endpoints[1].x, wall_height, wall.endpoints[1].y, 1};
        vec4 c{wall.endpoints[0].x, 0, wall.endpoints[0].y, 1};
        vec4 d{wall.endpoints[1].x, 0, wall.endpoints[1].y, 1};

        // translate
        a = translation * a;
        b = translation * b;
        c = translation * c;
        d = translation * d;

        constexpr auto reduce = [](vec4 &vec) {
            const float w = std::abs(vec.w);
            if (w) {
                vec.x /= w;
                vec.y /= w;
                vec.z /= w;
                vec.w /= w;
            }
        };

        // reduce
        reduce(a);
        reduce(b);
        reduce(c);
        reduce(d);

        // render a normalized triangle without validating its components
        const auto render_no_check = [&](const vec4 &vec_a, const vec4 &vec_b,
                                         const vec4 &vec_c) {
            const SDL_Vertex vertex_a{
                {width * (vec_a.x + 1), height * (-vec_a.y + 1)},
                wall.color.get()};
            const SDL_Vertex vertex_b{
                {width * (vec_b.x + 1), height * (-vec_b.y + 1)},
                wall.color.get()};
            const SDL_Vertex vertex_c{
                {width * (vec_c.x + 1), height * (-vec_c.y + 1)},
                wall.color.get()};
            renderer.render_triangles({vertex_a, vertex_b, vertex_c});
        };

        // render a normalized triangle with additional checks
        const auto render = [&](const vec4 &vec_a, const vec4 &vec_b,
                                const vec4 &vec_c) {
            const auto validate = [](const vec4 &a) -> bool {
                return std::abs(a.x) <= 1.0f && std::abs(a.y) <= 1.0f &&
                       std::abs(a.z) <= 1.0f;
            };
            if (vec_a.w > 0 && vec_b.w > 0 && vec_c.w > 0) {
                render_no_check(vec_a, vec_b, vec_c);
            } else if (vec_a.w > 0 || vec_b.w > 0 || vec_c.w > 0) {
            }
        };

        // render corresponding triangles
        render(a, b, c);
        render(b, c, d);

        if (!debug) {
            sdl::log_info("endpoints: (%.2f, %.2f), (%.2f, %.2f)",
                          wall.endpoints[0].x, wall.endpoints[0].y,
                          wall.endpoints[1].x, wall.endpoints[1].y);
            sdl::log_info("translated: (%.2f, %.2f, %.2f), (%.2f, %.2f, %.2f)",
                          a.x, a.y, a.z, b.x, b.y, b.z);
        }
    }
    debug = true;
}