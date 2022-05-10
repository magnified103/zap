#include <cmath>
#include <map>
#include <vector>

#include "geometry/matrix.hpp"
#include "geometry/trigonometric.hpp"
#include "opengl/gl.hpp"
#include "scenery/block.hpp"
#include "scenery/camera3d.hpp"
#include "scenery/texture.hpp"
#include "wrapper/logging.hpp"

void camera3d::rotate(float horizontal_delta, float vertical_delta) {
    horizontal_angle += horizontal_delta;
    vertical_angle += vertical_delta;
}

void camera3d::render(vec3 eye, vec3 up, const map3d &map,
                      const std::vector<GLuint> surface_ids, float screen_width,
                      float screen_height, GLuint shader_program) {

    float aspect = screen_width / screen_height;

    float fovy = 2 * std::atan(1.0f / aspect * std::tan(fov / 2));

    auto projection = perspective(fovy, aspect, near, far);

    vec3 direction(std::cos(horizontal_angle), std::tan(vertical_angle),
                   std::sin(horizontal_angle));

    auto view = look_at(eye, eye + direction, up);

    auto model = mat4{1.0f};

    mat4 MVP = projection * view * model;

    // map triangles to their associated surface id
    std::map<GLuint, std::pair<std::vector<vec3>, std::vector<vec2>>>
        map_triangles;

    for (const auto &tmp : map.terrain) {
        for (const auto &tile : tmp) {
            auto create_triangles_for_texture = [&](const vec3 &origin,
                                                    const texture &texture) {
                if (texture.surface_id < 0) {
                    return;
                }
                auto &[positions, UVs] =
                    map_triangles[surface_ids[texture.surface_id]];
                for (const auto &position : texture.positions) {
                    positions.push_back(origin + position);
                }
                UVs.insert(UVs.end(), texture.UVs.begin(), texture.UVs.end());
            };
            auto create_triangles_for_block = [&](vec3 origin,
                                                  const block &block) {
                origin.y += block.height_bottom;
                if (block.front_id >= 0) {
                    create_triangles_for_texture(origin,
                                                 map.textures[block.front_id]);
                }
                if (block.left_id >= 0) {
                    create_triangles_for_texture(origin,
                                                 map.textures[block.left_id]);
                }
                if (block.back_id >= 0) {
                    create_triangles_for_texture(origin,
                                                 map.textures[block.back_id]);
                }
                if (block.right_id >= 0) {
                    create_triangles_for_texture(origin,
                                                 map.textures[block.right_id]);
                }
                if (block.top_id >= 0) {
                    create_triangles_for_texture(origin,
                                                 map.textures[block.top_id]);
                }
                if (block.bottom_id >= 0) {
                    create_triangles_for_texture(origin,
                                                 map.textures[block.bottom_id]);
                }
            };
            if (tile.top_id >= 0) {
                create_triangles_for_block(tile.point_a,
                                           map.blocks[tile.top_id]);
            }
            if (tile.middle_id >= 0) {
                create_triangles_for_block(tile.point_a,
                                           map.blocks[tile.middle_id]);
            }
            if (tile.bottom_id >= 0) {
                create_triangles_for_block(tile.point_a,
                                           map.blocks[tile.bottom_id]);
            }
        }
    }

    // enable shader program
    glUseProgram(shader_program);

    // get texture location in program
    GLuint texture_loc =
        glGetUniformLocation(shader_program, "texture_sampler");

    // get mvp location in program
    GLuint MVP_loc = glGetUniformLocation(shader_program, "MVP");

    static bool debug = false;
    for (const auto &[surface_id, triangles] : map_triangles) {

        // i hate debug lol
        if (!debug) {
            debug = 1;
            sdl::log_info("projection [[%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f, "
                          "%.2f, %.2f], "
                          "[%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f, %.2f, %.2f]]",
                          projection[0].x, projection[0].y, projection[0].z,
                          projection[0].w, projection[1].x, projection[1].y,
                          projection[1].z, projection[1].w, projection[2].x,
                          projection[2].y, projection[2].z, projection[2].w,
                          projection[3].x, projection[3].y, projection[3].z,
                          projection[3].w);
            sdl::log_info(
                "view [[%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f, %.2f, %.2f], "
                "[%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f, %.2f, %.2f]]",
                view[0].x, view[0].y, view[0].z, view[0].w, view[1].x,
                view[1].y, view[1].z, view[1].w, view[2].x, view[2].y,
                view[2].z, view[2].w, view[3].x, view[3].y, view[3].z,
                view[3].w);
            sdl::log_info(
                "MVP [[%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f, %.2f, %.2f], "
                "[%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f, %.2f, %.2f]]",
                MVP[0].x, MVP[0].y, MVP[0].z, MVP[0].w, MVP[1].x, MVP[1].y,
                MVP[1].z, MVP[1].w, MVP[2].x, MVP[2].y, MVP[2].z, MVP[2].w,
                MVP[3].x, MVP[3].y, MVP[3].z, MVP[3].w);
            sdl::log_info("eye (%.2f, %.2f, %.2f)", eye.x, eye.y, eye.z);
            sdl::log_info("direction (%.2f, %.2f, %.2f)", direction.x,
                          direction.y, direction.z);
            for (const auto &pt : triangles.first) {
                const auto translated_pt = MVP * vec4{pt.x, pt.y, pt.z, 1};
                sdl::log_info("(%.2f, %.2f, %.2f) (%.2f, %.2f, %.2f, %.2f)",
                              pt.x, pt.y, pt.z, translated_pt.x,
                              translated_pt.y, translated_pt.z,
                              translated_pt.w);
            }
            for (const auto &uv : triangles.second) {
                sdl::log_info("(%.2f, %.2f)", uv.x, uv.y);
            }
        }

        // bind texture in GL_TEXTURE0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, surface_id);
        glUniform1i(texture_loc, 0);

        // bind MVP
        glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, &MVP[0][0]);

        // bind positions
        glEnableVertexAttribArray(0);
        GLuint position_buffer;
        glGenBuffers(1, &position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
        glBufferData(GL_ARRAY_BUFFER,
                     triangles.first.size() *
                         sizeof(decltype(triangles.first)::value_type),
                     triangles.first.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        // bind UVs
        glEnableVertexAttribArray(1);
        GLuint UV_buffer;
        glGenBuffers(1, &UV_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, UV_buffer);
        glBufferData(GL_ARRAY_BUFFER,
                     triangles.second.size() *
                         sizeof(decltype(triangles.second)::value_type),
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
}