#include "ppu_helper.h"

#include "core/immu.h"
#include "nes/nes.h"

#include <array>

namespace nesvis {

namespace {

const std::array<sf::Color, 64> kPalette{
        sf::Color(84, 84, 84, 255),
        sf::Color(0, 30, 116, 255),
        sf::Color(8, 16, 144, 255),
        sf::Color(48, 0, 136, 255),
        sf::Color(68, 0, 100, 255),
        sf::Color(92, 0, 48, 255),
        sf::Color(84, 4, 0, 255),
        sf::Color(60, 24, 0, 255),
        sf::Color(32, 42, 0, 255),
        sf::Color(8, 58, 0, 255),
        sf::Color(0, 64, 0, 255), // 0A
        sf::Color(0, 60, 0, 255),
        sf::Color(0, 50, 60, 255),
        sf::Color(0, 0, 0, 255),
        sf::Color(0, 0, 0, 255),
        sf::Color(0, 0, 0, 255), // 0F
        sf::Color(152, 150, 152, 255), // 10
        sf::Color(8, 76, 196, 255),
        sf::Color(48, 50, 236, 255),
        sf::Color(92, 30, 228, 255),
        sf::Color(136, 20, 176, 255),
        sf::Color(160, 20, 100, 255),
        sf::Color(152, 34, 32, 255),
        sf::Color(120, 60, 0, 255),
        sf::Color(84, 90, 0, 255),
        sf::Color(40, 114, 0, 255),
        sf::Color(8, 124, 0, 255),
        sf::Color(0, 118, 40, 255),
        sf::Color(0, 102, 120, 255),
        sf::Color(0, 0, 0, 255),
        sf::Color(0, 0, 0, 255),
        sf::Color(0, 0, 0, 255),
        sf::Color(236, 238, 236, 255), // 20
        sf::Color(76, 154, 236, 255),
        sf::Color(120, 124, 236, 255),
        sf::Color(176, 98, 236, 255),
        sf::Color(228, 84, 236, 255),
        sf::Color(236, 88, 180, 255),
        sf::Color(236, 106, 100, 255),
        sf::Color(212, 136, 32, 255),
        sf::Color(160, 170, 0, 255),
        sf::Color(116, 196, 0, 255),
        sf::Color(76, 208, 32, 255),
        sf::Color(56, 204, 108, 255),
        sf::Color(56, 180, 204, 255),
        sf::Color(60, 60, 60, 255),
        sf::Color(0, 0, 0, 255),
        sf::Color(0, 0, 0, 255), // 30
        sf::Color(236, 238, 236, 255),
        sf::Color(168, 204, 236, 255),
        sf::Color(188, 188, 236, 255),
        sf::Color(212, 178, 236, 255),
        sf::Color(236, 174, 236, 255),
        sf::Color(236, 174, 212, 255),
        sf::Color(236, 180, 176, 255),
        sf::Color(228, 196, 144, 255),
        sf::Color(204, 210, 120, 255),
        sf::Color(180, 222, 120, 255),
        sf::Color(168, 226, 144, 255),
        sf::Color(152, 226, 180, 255),
        sf::Color(160, 214, 228, 255),
        sf::Color(160, 162, 160, 255),
        sf::Color(0, 0, 0, 255),
        sf::Color(0, 0, 0, 255),
};

} // namespace

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
            const uint8_t bit_a = !!(a & mask);
            const uint8_t bit_b = !!(b & mask);
            const uint8_t color_index = bit_a | bit_b << 1u;

            const auto color = get_background_color(palette, color_index);
            image.setPixel(col, row, color);
        }
    }
    sf::Texture texture;
    texture.loadFromImage(image);
    return texture;
}

PpuHelper::NametableCell PpuHelper::get_nametable_cell(int x, int y) {
    // TODO(jn) handle mirroring
    const uint16_t address = 0x2000 + y * 32 + x;
    const uint8_t tile_index = nes_->ppu_mmu().read_byte(address);

    return {.tile_index = tile_index, .address = address};
}

sf::Color PpuHelper::get_color_from_index(uint16_t index) {
    return kPalette[index];
}

sf::Color PpuHelper::get_background_color(uint16_t color_set, uint16_t index) {
    const uint16_t color_address = 0x3F00 + color_set * 4 + index;
    const uint8_t color_index = nes_->ppu_mmu().read_byte(color_address);
    return get_color_from_index(color_index);
}

} // namespace nesvis
