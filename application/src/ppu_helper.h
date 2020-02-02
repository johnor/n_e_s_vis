#pragma once

#include <SFML/Graphics/Texture.hpp>

namespace n_e_s::nes {
class Nes;
} // namespace n_e_s::nes

namespace nesvis {

class PpuHelper {
public:
    struct NametableCell {
        uint16_t tile_index;
        uint16_t address;
    };

    explicit PpuHelper(n_e_s::nes::Nes *nes);

    sf::Texture get_pattern_table_texture(uint16_t pos,
            uint16_t pattern_table,
            uint16_t palette);

    PpuHelper::NametableCell get_nametable_cell(int x, int y);

    sf::Color get_color_from_index(uint16_t index);
    sf::Color get_background_color(uint16_t color_set, uint16_t index);

    static constexpr int kPaletteSize = 64;

private:
    n_e_s::nes::Nes *nes_{nullptr};
};

} // namespace nesvis
