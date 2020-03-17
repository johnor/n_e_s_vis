#include "ppu_helper.h"

#include "core/immu.h"
#include "nes/nes.h"

namespace nesvis {

PpuHelper::PpuHelper(n_e_s::nes::Nes *nes) : nes_{nes} {}

sf::Texture PpuHelper::get_pattern_table_texture(uint16_t pos,
        uint16_t pattern_table,
        uint16_t palette) {
    sf::Image image;
    image.create(8, 8, sf::Color(10, 100, 0));

    for (uint8_t row = 0; row < 8; ++row) {
        // Second pattern table starts at 0x1000
        const uint16_t base_address = pattern_table * 0x1000 + pos + row;
        const uint8_t a = nes_->ppu_mmu().read_byte(base_address);
        const uint8_t b = nes_->ppu_mmu().read_byte(base_address + 8u);

        for (uint8_t col = 0; col < 8u; ++col) {
            // First column is the leftmost bit
            const uint16_t mask = 1u << (7u - col);
            const uint8_t color_index = !!(a & mask) + !!(b & mask);

            const auto color = get_color_from_index(palette, color_index);
            image.setPixel(col, row, color);
        }
    }
    sf::Texture texture;
    texture.loadFromImage(image);
    return texture;
}

PpuHelper::CellData PpuHelper::get_cell_data_from_nametable(int x, int y) {
    // TODO(jn) handle mirroring
    const uint16_t address = 0x2000 + y * 32 + x;
    const uint8_t tile_index = nes_->ppu_mmu().read_byte(address);

    return {.tile_index = tile_index, .address = address};
}

sf::Color PpuHelper::get_color_from_index([[maybe_unused]] uint16_t palette,
        uint16_t index) {
    // TODO(jn): implement palette
    sf::Color color = sf::Color::Black;
    switch (index) {
    case 1:
        color = sf::Color(84, 84, 84);
        break;
    case 2:
        color = sf::Color(0, 30, 116);
        break;
    case 3:
        color = sf::Color(8, 16, 144);
        break;
    }
    return color;
}

} // namespace nesvis
