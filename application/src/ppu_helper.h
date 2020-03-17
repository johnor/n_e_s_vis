#pragma once

#include <SFML/Graphics/Texture.hpp>

namespace n_e_s::nes {
class Nes;
} // namespace n_e_s::nes

namespace nesvis {

class PpuHelper {
public:
    struct CellData {
        uint16_t tile_index;
        uint16_t address;
    };

    explicit PpuHelper(n_e_s::nes::Nes *nes);

    sf::Texture get_pattern_table_texture(uint16_t pos,
            uint16_t pattern_table,
            uint16_t palette);

    PpuHelper::CellData get_cell_data_from_nametable(int x, int y);

    sf::Color get_color_from_index(uint16_t palette, uint16_t index);

private:
    n_e_s::nes::Nes *nes_{nullptr};
};

} // namespace nesvis
