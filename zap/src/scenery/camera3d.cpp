#include <cmath>
#include <map>
#include <vector>

#include "geometry/matrix.hpp"
#include "geometry/trigonometric.hpp"
#include "geometry/vector.hpp"
#include "opengl/gl.hpp"
#include "scenery/block.hpp"
#include "scenery/camera3d.hpp"
#include "scenery/map3d.hpp"
#include "scenery/texture.hpp"
#include "sdl2/handlers/logging.hpp"

vec3 camera3d::get_view_vector() {
    return vec3{std::cos(horizontal_angle), std::tan(vertical_angle), std::sin(horizontal_angle)};
}

void camera3d::rotate(float horizontal_delta, float vertical_delta) {
    horizontal_angle += horizontal_delta;
    vertical_angle += vertical_delta;
}

void camera3d::render(const vec3 &eye, const vec3 &up, const map3d &map,
                      const std::vector<GLuint> &surface_ids, const float &screen_width,
                      const float &screen_height, const GLuint &shader_program) {

    float aspect = screen_width / screen_height;

    float fovy = 2 * std::atan(1.0f / aspect * std::tan(fov / 2));

    auto projection = perspective(fovy, aspect, near, far);

    vec3 direction = normalize(get_view_vector());

    auto view = look_at(eye, eye + direction, up);

    auto model = mat4{1.0f};

    mat4 MVP = projection * view * model;

    // map triangles to their associated surface id
    std::map<int, std::pair<std::vector<vec3>, std::vector<vec2>>> map_triangles;

    // render walls and obstacles
    for (int col = 0; col < map.number_of_columns; col++) {
        for (int row = 0; row < map.number_of_rows; row++) {

            const auto &tile = map.terrain[col][row];

            vec3 tile_origin{col * map.cell_size, 0, row * map.cell_size};

            auto feed_to_renderer = [&](const vec3 &position, const mesh &mesh) {
                for (const auto &triangle : mesh.faces) {
                    auto &[positions, UVs] = map_triangles[triangle.texture_index];
                    for (const auto &index : triangle.vertex_indices) {
                        positions.push_back(mesh.vertices[index] + position);
                    }
                    UVs.insert(UVs.end(), triangle.UVs.begin(), triangle.UVs.end());
                }
            };

            if (tile.top_mesh_index >= 0) {
                tile_origin.y = tile.ceil_level;
                feed_to_renderer(tile_origin, map.meshes[tile.top_mesh_index]);
            }
            if (tile.middle_mesh_index >= 0) {
                tile_origin.y = tile.floor_level;
                feed_to_renderer(tile_origin, map.meshes[tile.middle_mesh_index]);
            }
            if (tile.bottom_mesh_index >= 0) {
                tile_origin.y = tile.floor_level;
                feed_to_renderer(tile_origin, map.meshes[tile.bottom_mesh_index]);
            }
        }
    }

    // render projectiles
    for (const auto &projectile : map.projectiles) {
        if (projectile.phase >= 2) {
            continue;
        }
        vec3 up_ray = up - direction * (dot(direction, up) / dot(direction, direction));
        const vec4 temp = ::rotate(mat4{1}, radians(-90.0f), direction) * vec4{up_ray, 1};
        vec3 left_ray{temp.x, temp.y, temp.z};

        // normalize axis
        up_ray = normalize(up_ray) *
                 (projectile.phase == 0 ? projectile.base.move_size : projectile.base.end_size);
        left_ray = normalize(left_ray) *
                   (projectile.phase == 0 ? projectile.base.move_size : projectile.base.end_size);

        auto &[positions, UVs] =
            map_triangles[projectile.phase == 0 ? projectile.base.move_id : projectile.base.end_id];
        positions.push_back(projectile.position + left_ray - up_ray);
        positions.push_back(projectile.position + left_ray + up_ray);
        positions.push_back(projectile.position - left_ray - up_ray);
        positions.push_back(projectile.position + left_ray + up_ray);
        positions.push_back(projectile.position - left_ray - up_ray);
        positions.push_back(projectile.position - left_ray + up_ray);

        // their associated UVs
        UVs.push_back({0, 0});
        UVs.push_back({0, 1});
        UVs.push_back({1, 0});
        UVs.push_back({0, 1});
        UVs.push_back({1, 0});
        UVs.push_back({1, 1});
    }

    auto render_triangles = [&]() {
        // enable shader program
        glUseProgram(shader_program);

        // get texture location in program
        GLuint texture_loc = glGetUniformLocation(shader_program, "texture_sampler");

        // get mvp location in program
        GLuint MVP_loc = glGetUniformLocation(shader_program, "MVP");

        static bool debug = true;
        for (const auto &[frame_id, triangles] : map_triangles) {

            if (frame_id < 0) {
                continue;
            }

            // i hate debug lol
            if (!debug) {
                debug = 1;
                sdl::log_info("projection [[%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f, "
                              "%.2f, %.2f], "
                              "[%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f, %.2f, %.2f]]",
                              projection[0].x, projection[0].y, projection[0].z, projection[0].w,
                              projection[1].x, projection[1].y, projection[1].z, projection[1].w,
                              projection[2].x, projection[2].y, projection[2].z, projection[2].w,
                              projection[3].x, projection[3].y, projection[3].z, projection[3].w);
                sdl::log_info("view [[%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f, %.2f, %.2f], "
                              "[%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f, %.2f, %.2f]]",
                              view[0].x, view[0].y, view[0].z, view[0].w, view[1].x, view[1].y,
                              view[1].z, view[1].w, view[2].x, view[2].y, view[2].z, view[2].w,
                              view[3].x, view[3].y, view[3].z, view[3].w);
                sdl::log_info("MVP [[%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f, %.2f, %.2f], "
                              "[%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f, %.2f, %.2f]]",
                              MVP[0].x, MVP[0].y, MVP[0].z, MVP[0].w, MVP[1].x, MVP[1].y, MVP[1].z,
                              MVP[1].w, MVP[2].x, MVP[2].y, MVP[2].z, MVP[2].w, MVP[3].x, MVP[3].y,
                              MVP[3].z, MVP[3].w);
                sdl::log_info("eye (%.2f, %.2f, %.2f)", eye.x, eye.y, eye.z);
                sdl::log_info("direction (%.2f, %.2f, %.2f)", direction.x, direction.y,
                              direction.z);
                for (const auto &pt : triangles.first) {
                    const auto translated_pt = MVP * vec4{pt.x, pt.y, pt.z, 1};
                    sdl::log_info("(%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f, %.2f)", pt.x, pt.y, pt.z,
                                  translated_pt.x, translated_pt.y, translated_pt.z,
                                  translated_pt.w);
                }
                for (const auto &uv : triangles.second) {
                    sdl::log_info("(%.2f, %.2f)", uv.x, uv.y);
                }
            }

            // bind texture in GL_TEXTURE0
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, surface_ids[frame_id]);
            glUniform1i(texture_loc, 0);

            // bind MVP
            glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, &MVP[0][0]);

            // bind positions
            glEnableVertexAttribArray(0);
            GLuint position_buffer;
            glGenBuffers(1, &position_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
            glBufferData(GL_ARRAY_BUFFER,
                         triangles.first.size() * sizeof(decltype(triangles.first)::value_type),
                         triangles.first.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

            // bind UVs
            glEnableVertexAttribArray(1);
            GLuint UV_buffer;
            glGenBuffers(1, &UV_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, UV_buffer);
            glBufferData(GL_ARRAY_BUFFER,
                         triangles.second.size() * sizeof(decltype(triangles.second)::value_type),
                         triangles.second.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

            // draw triangles
            glDrawArrays(GL_TRIANGLES, 0, triangles.first.size());

            // disable stuffs
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDeleteBuffers(1, &position_buffer);
            glDeleteBuffers(1, &UV_buffer);
        }
    };

    render_triangles();
    map_triangles.clear();

    // render monsters
    for (const auto &monster : map.monsters) {
        vec3 direction_ray{direction.x, 0, direction.z};
        vec3 up_ray = up;
        const vec4 temp = ::rotate(mat4{1}, radians(-90.0f), direction_ray) * vec4{up_ray, 1};
        vec3 left_ray{temp.x, temp.y, temp.z};

        // normalize axis
        up_ray = normalize(up_ray) * monster.hitbox_height * 0.5;
        left_ray = normalize(left_ray) * monster.hitbox_radius;

        auto position = monster.current_movement.position;
        position.y += monster.hitbox_height * 0.5;

        int frame_id = -1;
        if (monster.animation_phase == 0 &&
            monster.moving.current_phase < static_cast<int>(monster.moving.frame_id.size())) {
            frame_id = monster.moving.frame_id[monster.moving.current_phase];
        }
        if (monster.animation_phase == 1 &&
            monster.shooting.current_phase < static_cast<int>(monster.shooting.frame_id.size())) {
            frame_id = monster.shooting.frame_id[monster.shooting.current_phase];
        }
        if (frame_id == -1) {
            continue;
        }
        auto &[positions, UVs] = map_triangles[frame_id];
        positions.push_back(position + left_ray - up_ray);
        positions.push_back(position + left_ray + up_ray);
        positions.push_back(position - left_ray - up_ray);
        positions.push_back(position + left_ray + up_ray);
        positions.push_back(position - left_ray - up_ray);
        positions.push_back(position - left_ray + up_ray);

        // their associated UVs
        UVs.push_back({0, 0});
        UVs.push_back({0, 1});
        UVs.push_back({1, 0});
        UVs.push_back({0, 1});
        UVs.push_back({1, 0});
        UVs.push_back({1, 1});
    }
    render_triangles();
}