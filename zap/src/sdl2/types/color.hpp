#ifndef SDL2_COLOR_HPP
#define SDL2_COLOR_HPP

#include <SDL.h>
#include <cereal/cereal.hpp>

#include "sdl2/detail/common.hpp"

namespace sdl {

class color {
public:
    constexpr color() : sdl_color{} {};

    constexpr color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : sdl_color{r, g, b, a} {}

    constexpr color(Uint8 r, Uint8 g, Uint8 b) : sdl_color{r, g, b, 0xff} {}

    SDL_Color get() noexcept { return sdl_color; }

    const SDL_Color &get() const noexcept { return sdl_color; }

    SDL_Color *get_ptr() noexcept { return &sdl_color; }

    const SDL_Color *get_ptr() const noexcept { return &sdl_color; }

    Uint8 red() const { return sdl_color.r; }

    Uint8 &red() { return sdl_color.r; }

    Uint8 green() const { return sdl_color.g; }

    Uint8 &green() { return sdl_color.g; }

    Uint8 blue() const { return sdl_color.b; }

    Uint8 &blue() { return sdl_color.b; }

    Uint8 alpha() const { return sdl_color.a; }

    Uint8 &alpha() { return sdl_color.a; }

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(
            cereal::make_nvp<Uint8>("r", sdl_color.r), cereal::make_nvp<Uint8>("g", sdl_color.g),
            cereal::make_nvp<Uint8>("b", sdl_color.b), cereal::make_nvp<Uint8>("a", sdl_color.a));
    }

private:
    SDL_Color sdl_color;
};

namespace colors {

inline constexpr color alice_blue{0xf0, 0xf8, 0xff};
inline constexpr color antique_white{0xfa, 0xeb, 0xd7};
inline constexpr color aqua{0x00, 0xff, 0xff};
inline constexpr color aquamarine{0x7f, 0xff, 0xd4};
inline constexpr color azure{0xf0, 0xff, 0xff};
inline constexpr color beige{0xf5, 0xf5, 0xdc};
inline constexpr color bisque{0xff, 0xe4, 0xc4};
inline constexpr color black{0x00, 0x00, 0x00};
inline constexpr color blanched_almond{0xff, 0xeb, 0xcd};
inline constexpr color blue{0x00, 0x00, 0xff};
inline constexpr color blue_violet{0x8a, 0x2b, 0xe2};
inline constexpr color brown{0xa5, 0x2a, 0x2a};
inline constexpr color burly_wood{0xde, 0xb8, 0x87};
inline constexpr color cadet_blue{0x5f, 0x9e, 0xa0};
inline constexpr color chartreuse{0x7f, 0xff, 0x00};
inline constexpr color chocolate{0xd2, 0x69, 0x1e};
inline constexpr color coral{0xff, 0x7f, 0x50};
inline constexpr color cornflower_blue{0x64, 0x95, 0xed};
inline constexpr color cornsilk{0xff, 0xf8, 0xdc};
inline constexpr color crimson{0xdc, 0x14, 0x3c};
inline constexpr color cyan{0x00, 0xff, 0xff};
inline constexpr color dark_blue{0x00, 0x00, 0x8b};
inline constexpr color dark_cyan{0x00, 0x8b, 0x8b};
inline constexpr color dark_golden_rod{0xb8, 0x86, 0x0b};
inline constexpr color dark_gray{0xa9, 0xa9, 0xa9};
inline constexpr color dark_grey{0xa9, 0xa9, 0xa9};
inline constexpr color dark_green{0x00, 0x64, 0x00};
inline constexpr color dark_khaki{0xbd, 0xb7, 0x6b};
inline constexpr color dark_magenta{0x8b, 0x00, 0x8b};
inline constexpr color dark_olive_green{0x55, 0x6b, 0x2f};
inline constexpr color dark_orange{0xff, 0x8c, 0x00};
inline constexpr color dark_orchid{0x99, 0x32, 0xcc};
inline constexpr color dark_red{0x8b, 0x00, 0x00};
inline constexpr color dark_salmon{0xe9, 0x96, 0x7a};
inline constexpr color dark_sea_green{0x8f, 0xbc, 0x8f};
inline constexpr color dark_slate_blue{0x48, 0x3d, 0x8b};
inline constexpr color dark_slate_gray{0x2f, 0x4f, 0x4f};
inline constexpr color dark_slate_grey{0x2f, 0x4f, 0x4f};
inline constexpr color dark_turquoise{0x00, 0xce, 0xd1};
inline constexpr color dark_violet{0x94, 0x00, 0xd3};
inline constexpr color deep_pink{0xff, 0x14, 0x93};
inline constexpr color deep_sky_blue{0x00, 0xbf, 0xff};
inline constexpr color dim_gray{0x69, 0x69, 0x69};
inline constexpr color dim_grey{0x69, 0x69, 0x69};
inline constexpr color dodger_blue{0x1e, 0x90, 0xff};
inline constexpr color fire_brick{0xb2, 0x22, 0x22};
inline constexpr color floral_white{0xff, 0xfa, 0xf0};
inline constexpr color forest_green{0x22, 0x8b, 0x22};
inline constexpr color fuchsia{0xff, 0x00, 0xff};
inline constexpr color gainsboro{0xdc, 0xdc, 0xdc};
inline constexpr color ghost_white{0xf8, 0xf8, 0xff};
inline constexpr color gold{0xff, 0xd7, 0x00};
inline constexpr color golden_rod{0xda, 0xa5, 0x20};
inline constexpr color gray{0x80, 0x80, 0x80};
inline constexpr color grey{0x80, 0x80, 0x80};
inline constexpr color green{0x00, 0x80, 0x00};
inline constexpr color green_yellow{0xad, 0xff, 0x2f};
inline constexpr color honey_dew{0xf0, 0xff, 0xf0};
inline constexpr color hot_pink{0xff, 0x69, 0xb4};
inline constexpr color indian_red{0xcd, 0x5c, 0x5c};
inline constexpr color indigo{0x4b, 0x00, 0x82};
inline constexpr color ivory{0xff, 0xff, 0xf0};
inline constexpr color khaki{0xf0, 0xe6, 0x8c};
inline constexpr color lavender{0xe6, 0xe6, 0xfa};
inline constexpr color lavender_blush{0xff, 0xf0, 0xf5};
inline constexpr color lawn_green{0x7c, 0xfc, 0x00};
inline constexpr color lemon_chiffon{0xff, 0xfa, 0xcd};
inline constexpr color light_blue{0xad, 0xd8, 0xe6};
inline constexpr color light_coral{0xf0, 0x80, 0x80};
inline constexpr color light_cyan{0xe0, 0xff, 0xff};
inline constexpr color light_golden_rod_yellow{0xfa, 0xfa, 0xd2};
inline constexpr color light_gray{0xd3, 0xd3, 0xd3};
inline constexpr color light_grey{0xd3, 0xd3, 0xd3};
inline constexpr color light_green{0x90, 0xee, 0x90};
inline constexpr color light_pink{0xff, 0xb6, 0xc1};
inline constexpr color light_salmon{0xff, 0xa0, 0x7a};
inline constexpr color light_sea_green{0x20, 0xb2, 0xaa};
inline constexpr color light_sky_blue{0x87, 0xce, 0xfa};
inline constexpr color light_slate_gray{0x77, 0x88, 0x99};
inline constexpr color light_slate_grey{0x77, 0x88, 0x99};
inline constexpr color light_steel_blue{0xb0, 0xc4, 0xde};
inline constexpr color light_yellow{0xff, 0xff, 0xe0};
inline constexpr color lime{0x00, 0xff, 0x00};
inline constexpr color lime_green{0x32, 0xcd, 0x32};
inline constexpr color linen{0xfa, 0xf0, 0xe6};
inline constexpr color magenta{0xff, 0x00, 0xff};
inline constexpr color maroon{0x80, 0x00, 0x00};
inline constexpr color medium_aqua_marine{0x66, 0xcd, 0xaa};
inline constexpr color medium_blue{0x00, 0x00, 0xcd};
inline constexpr color medium_orchid{0xba, 0x55, 0xd3};
inline constexpr color medium_purple{0x93, 0x70, 0xdb};
inline constexpr color medium_sea_green{0x3c, 0xb3, 0x71};
inline constexpr color medium_slate_blue{0x7b, 0x68, 0xee};
inline constexpr color medium_spring_green{0x00, 0xfa, 0x9a};
inline constexpr color medium_turquoise{0x48, 0xd1, 0xcc};
inline constexpr color medium_violet_red{0xc7, 0x15, 0x85};
inline constexpr color midnight_blue{0x19, 0x19, 0x70};
inline constexpr color mint_cream{0xf5, 0xff, 0xfa};
inline constexpr color misty_rose{0xff, 0xe4, 0xe1};
inline constexpr color moccasin{0xff, 0xe4, 0xb5};
inline constexpr color navajo_white{0xff, 0xde, 0xad};
inline constexpr color navy{0x00, 0x00, 0x80};
inline constexpr color old_lace{0xfd, 0xf5, 0xe6};
inline constexpr color olive{0x80, 0x80, 0x00};
inline constexpr color olive_drab{0x6b, 0x8e, 0x23};
inline constexpr color orange{0xff, 0xa5, 0x00};
inline constexpr color orange_red{0xff, 0x45, 0x00};
inline constexpr color orchid{0xda, 0x70, 0xd6};
inline constexpr color pale_golden_rod{0xee, 0xe8, 0xaa};
inline constexpr color pale_green{0x98, 0xfb, 0x98};
inline constexpr color pale_turquoise{0xaf, 0xee, 0xee};
inline constexpr color pale_violet_red{0xdb, 0x70, 0x93};
inline constexpr color papaya_whip{0xff, 0xef, 0xd5};
inline constexpr color peach_puff{0xff, 0xda, 0xb9};
inline constexpr color peru{0xcd, 0x85, 0x3f};
inline constexpr color pink{0xff, 0xc0, 0xcb};
inline constexpr color plum{0xdd, 0xa0, 0xdd};
inline constexpr color powder_blue{0xb0, 0xe0, 0xe6};
inline constexpr color purple{0x80, 0x00, 0x80};
inline constexpr color rebecca_purple{0x66, 0x33, 0x99};
inline constexpr color red{0xff, 0x00, 0x00};
inline constexpr color rosy_brown{0xbc, 0x8f, 0x8f};
inline constexpr color royal_blue{0x41, 0x69, 0xe1};
inline constexpr color saddle_brown{0x8b, 0x45, 0x13};
inline constexpr color salmon{0xfa, 0x80, 0x72};
inline constexpr color sandy_brown{0xf4, 0xa4, 0x60};
inline constexpr color sea_green{0x2e, 0x8b, 0x57};
inline constexpr color sea_shell{0xff, 0xf5, 0xee};
inline constexpr color sienna{0xa0, 0x52, 0x2d};
inline constexpr color silver{0xc0, 0xc0, 0xc0};
inline constexpr color sky_blue{0x87, 0xce, 0xeb};
inline constexpr color slate_blue{0x6a, 0x5a, 0xcd};
inline constexpr color slate_gray{0x70, 0x80, 0x90};
inline constexpr color slate_grey{0x70, 0x80, 0x90};
inline constexpr color snow{0xff, 0xfa, 0xfa};
inline constexpr color spring_green{0x00, 0xff, 0x7f};
inline constexpr color steel_blue{0x46, 0x82, 0xb4};
inline constexpr color tan{0xd2, 0xb4, 0x8c};
inline constexpr color teal{0x00, 0x80, 0x80};
inline constexpr color thistle{0xd8, 0xbf, 0xd8};
inline constexpr color tomato{0xff, 0x63, 0x47};
inline constexpr color turquoise{0x40, 0xe0, 0xd0};
inline constexpr color violet{0xee, 0x82, 0xee};
inline constexpr color wheat{0xf5, 0xde, 0xb3};
inline constexpr color white{0xff, 0xff, 0xff};
inline constexpr color white_smoke{0xf5, 0xf5, 0xf5};
inline constexpr color yellow{0xff, 0xff, 0x00};
inline constexpr color yellow_green{0x9a, 0xcd, 0x32};

} // namespace colors

} // namespace sdl

#endif // SDL2_COLOR_HPP