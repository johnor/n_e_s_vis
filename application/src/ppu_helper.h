#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <array>

namespace n_e_s::nes {
class Nes;
} // namespace n_e_s::nes

namespace nesvis {

class PpuHelper {
public:
    struct PatterntableCell {
        std::array<uint8_t, 8 * 8> data;
        uint16_t address;
    };
    struct NametableCell {
        uint16_t tile_index;
        uint16_t address;
    };
    struct AttributeCell {
        uint16_t attribute;
        uint8_t palette;
        uint16_t address;
    };
    struct Sprite {
        uint8_t x;
        uint8_t y;
        uint8_t tile_index;
        uint8_t attribute;

        uint8_t palette;
        bool front_of_bkg;
        bool flip_horizontally;
        bool flip_vertically;
    };

    explicit PpuHelper(n_e_s::nes::Nes *nes);

    sf::Texture get_pattern_table_texture(uint16_t pos,
            uint16_t pattern_table,
            uint16_t palette);

    PatterntableCell get_pattern_table_cell(uint16_t pos,
            uint16_t pattern_table);

    PpuHelper::NametableCell get_nametable_cell(int x, int y);
    PpuHelper::AttributeCell get_attribute_cell(int x, int y);
    PpuHelper::Sprite get_sprite(uint8_t sprite_index);

    sf::Color get_color_from_index(uint16_t index);
    sf::Color get_background_color(uint16_t palette, uint8_t index);

    static constexpr int kPaletteSize = 64;

private:
    n_e_s::nes::Nes *nes_{nullptr};
};

} // namespace nesvis
