#ifndef SCENERY_HUD_HPP
#define SCENERY_HUD_HPP

#include <string>
#include <type_traits>
#include <vector>

#include <cereal/cereal.hpp>

#include "entity/inventory.hpp"
#include "geometry/vector.hpp"
#include "opengl/gl.hpp"
#include "opengl/utilities.hpp"
#include "sdl2/types/font.hpp"

inline void render2d(const std::vector<vec2> &positions, const std::vector<vec2> &UVs,
                     const GLuint &texture_id, const GLuint &shader_program) {
    // enable shader program
    glUseProgram(shader_program);

    // get texture location in program
    GLuint texture_loc = glGetUniformLocation(shader_program, "texture_sampler");

    // bind texture in GL_TEXTURE0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glUniform1i(texture_loc, 0);

    glEnableVertexAttribArray(0);
    GLuint position_buffer;
    glGenBuffers(1, &position_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 positions.size() * sizeof(std::remove_cvref_t<decltype(positions)>::value_type),
                 positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // bind UVs
    glEnableVertexAttribArray(1);
    GLuint UV_buffer;
    glGenBuffers(1, &UV_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, UV_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 UVs.size() * sizeof(std::remove_cvref_t<decltype(UVs)>::value_type), UVs.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // draw triangles
    glDrawArrays(GL_TRIANGLES, 0, positions.size());

    // disable stuffs
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDeleteBuffers(1, &position_buffer);
    glDeleteBuffers(1, &UV_buffer);
}

void render_2d_rectangle(const vec2 &bottom_left, const vec2 &top_right, const GLuint &texture_id,
                         const GLuint &shader_program) {
    const vec2 top_left{bottom_left.x, top_right.y};
    const vec2 bottom_right{top_right.x, bottom_left.y};
    render2d({bottom_left, top_left, bottom_right, top_left, bottom_right, top_right},
             {{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}},
             texture_id, shader_program);
}

struct monitor {
    int icon_texture_index;
    float height;
    float width;
    float icon_width;
    sdl::color text_color;

    void render(const std::string &text, const sdl::font &font,
                const std::vector<GLuint> &texture_ids, const float &screen_width,
                const float &screen_height, const GLuint &shader_program) {
        // scaled coordinates
        const float scaled_height = height / screen_height * 2.0f;
        const float scaled_width = width / screen_width * 2.0f;
        const float scaled_icon_width = icon_width / screen_width * 2.0f;

        // render icon
        GLuint icon_texture_id = texture_ids[icon_texture_id];
        render_2d_rectangle(vec2{-1.0f, -1.0f},
                            vec2{-1.0f + scaled_icon_width, -1.0f + scaled_height}, icon_texture_id,
                            shader_program);

        // render text
        GLuint text_texture_id = load_texture(font.render_utf8_solid(text, text_color));
        render_2d_rectangle(vec2{-1.0f + scaled_icon_width, -1.0f},
                            vec2{-1.0f + scaled_width, -1.0f + scaled_height}, text_texture_id,
                            shader_program);
    }

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(icon_texture_index), CEREAL_NVP(height), CEREAL_NVP(width),
                CEREAL_NVP(icon_width), CEREAL_NVP(text_color));
    }
};

struct hotbar {
    float sep;
    float icon_width;
    float icon_height;

    void render(const inventory &inventory, const std::vector<GLuint> &texture_ids,
                const float &screen_width, const float &screen_height,
                const GLuint &shader_program) {
        const vec2 scaled_icon_size{icon_width / screen_width * 2.0f,
                                    icon_height / screen_height * 2.0f};
        const float scaled_sep = sep / screen_width * 2.0f;

        vec2 pos{1.0f - scaled_icon_size.x, -1.0f};
        const auto &items = inventory.items;
        for (int i = 0; i < static_cast<int>(items.size()); i++) {
            if (items[i]) {
                if (i == inventory.selection) {
                    render_2d_rectangle(pos, pos + scaled_icon_size,
                                        items[i]->highlighted_texture_index, shader_program);
                } else {
                    render_2d_rectangle(pos, pos + scaled_icon_size,
                                        items[i]->regular_texture_index, shader_program);
                }
                pos.x -= scaled_sep;
            }
        }
    }

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(sep), CEREAL_NVP(icon_width), CEREAL_NVP(icon_height));
    }
};

struct hud {
    monitor health_bar;
    monitor ammo_bar;
    std::string font_path;
    hotbar hotbar;

    // runtime specific
    sdl::font font;

    void render(int hp, int ammo, const sdl::font &font, const inventory &inventory,
                const std::vector<GLuint> &texture_ids, const float &screen_width,
                const float &screen_height, const GLuint &shader_program) {
        health_bar.render(std::to_string(hp), font, texture_ids, screen_width, screen_height,
                          shader_program);
        ammo_bar.render(std::to_string(ammo), font, texture_ids, screen_width, screen_height,
                        shader_program);
        hotbar.render(inventory, texture_ids, screen_width, screen_height, shader_program);
    }

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(health_bar), CEREAL_NVP(ammo_bar), CEREAL_NVP(font_path),
                CEREAL_NVP(hotbar));
    }
};

#endif // SCENERY_HUD_HPP