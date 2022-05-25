#ifndef SCENERY_HUD_HPP
#define SCENERY_HUD_HPP

#include <memory>
#include <string>
#include <vector>

#include <cereal/cereal.hpp>

#include "entity/inventory.hpp"
#include "geometry/vector.hpp"
#include "opengl/gl.hpp"
#include "sdl2/types/font.hpp"

void render2d(const std::vector<vec2> &positions, const std::vector<vec2> &UVs,
              const GLuint &texture_id, const GLuint &shader_program);

void render_2d_rectangle(const vec2 &bottom_left, const vec2 &top_right, const GLuint &texture_id,
                         const GLuint &shader_program);

struct monitor {
    int icon_texture_index;
    float height;
    float per_letter_width;
    float icon_width;
    sdl::color text_color;

    void render(const std::string &text, const sdl::font &font, const float &height_offset,
                const std::vector<GLuint> &texture_ids, const float &screen_width,
                const float &screen_height, const GLuint &shader_program);

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(icon_texture_index), CEREAL_NVP(height), CEREAL_NVP(per_letter_width),
                CEREAL_NVP(icon_width), CEREAL_NVP(text_color));
    }
};

struct hotbar {
    float sep;
    float icon_width;
    float icon_height;

    void render(const inventory &inventory, const std::vector<GLuint> &texture_ids,
                const float &screen_width, const float &screen_height,
                const GLuint &shader_program);

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
    hotbar inventory_bar;
    int death_screen_texture_index;
    std::string death_message;
    sdl::color death_message_color;
    float death_message_height;
    float per_letter_death_message_width;

    // runtime specific
    std::unique_ptr<sdl::font> font;

    void render_scene(int hp, int ammo, const inventory &inventory,
                      const std::vector<GLuint> &texture_ids, const float &screen_width,
                      const float &screen_height, const GLuint &shader_program);

    void render_end_screen(const std::vector<GLuint> &texture_ids, const float &screen_width,
                           const float &screen_height, const GLuint shader_program);

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(health_bar), CEREAL_NVP(ammo_bar), CEREAL_NVP(font_path),
                CEREAL_NVP(inventory_bar), CEREAL_NVP(death_screen_texture_index),
                CEREAL_NVP(death_message), CEREAL_NVP(death_message_color),
                CEREAL_NVP(death_message_height), CEREAL_NVP(per_letter_death_message_width));
    }
};

#endif // SCENERY_HUD_HPP