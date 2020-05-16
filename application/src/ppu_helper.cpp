#include "ppu_helper.h"

#include "nes/core/immu.h"
#include "nes/core/ippu.h"
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
        const uint16_t base_address = pattern_table * 0x1000 + pos * 16 + row;
        const uint8_t a = nes_->ppu_mmu().read_byte(base_address);
        const uint8_t b = nes_->ppu_mmu().read_byte(base_address + 8u);

        for (uint8_t col = 0; col < 8u; ++col) {
            // First column is the leftmost bit
            const uint16_t mask = 1u << (7u - col);
            const uint8_t bit_a = !!(a & mask);
            const uint8_t bit_b = !!(b & mask);
            const uint8_t color_index =
                    bit_a | static_cast<uint8_t>(bit_b << 1u);

            const auto color = get_background_color(palette, color_index);
            image.setPixel(col, row, color);
        }
    }
    sf::Texture texture;
    texture.loadFromImage(image);
    return texture;
}

PpuHelper::PatterntableCell PpuHelper::get_pattern_table_cell(uint16_t pos,
        uint16_t pattern_table) {
    PpuHelper::PatterntableCell cell;

    for (uint8_t row = 0; row < 8; ++row) {
        // Second pattern table starts at 0x1000
        const uint16_t base_address = pattern_table * 0x1000 + pos * 16 + row;
        const uint8_t a = nes_->ppu_mmu().read_byte(base_address);
        const uint8_t b = nes_->ppu_mmu().read_byte(base_address + 8u);

        for (uint8_t col = 0; col < 8u; ++col) {
            // First column is the leftmost bit
            const uint16_t mask = 1u << (7u - col);
            const uint8_t bit_a = !!(a & mask);
            const uint8_t bit_b = !!(b & mask);
            const uint8_t color_index =
                    bit_a | static_cast<uint8_t>(bit_b << 1u);

            const uint8_t pixel_index = row * 8 + col;
            cell.data[pixel_index] = color_index;
        }
    }
    return cell;
}

PpuHelper::NametableCell PpuHelper::get_nametable_cell(int x,
        int y,
        uint16_t nametable) {
    // $2000-$23FF   $0400 Nametable 0
    // $2400-$27FF   $0400 Nametable 1
    // $2800-$2BFF   $0400 Nametable 2
    // $2C00-$2FFF   $0400 Nametable 3
    // $3000-$3EFF   $0F00 Mirrors of $2000-$2EFF
    const uint16_t address = 0x2000 + 0x0400 * nametable + y * 32 + x;
    const uint8_t tile_index = nes_->ppu_mmu().read_byte(address);

    return {.tile_index = tile_index, .address = address};
}

PpuHelper::AttributeCell PpuHelper::get_attribute_cell(int x, int y) {
    // TODO(jn) handle mirroring, this will not work
    // Each attribute cell has 4 tiles in it
    const int attribute_x = x / 4;
    const int attribute_y = y / 4;
    const uint16_t address = 0x23C0 + attribute_y * 8 + attribute_x;
    const uint8_t attribute_raw = nes_->ppu_mmu().read_byte(address);

    // Each attribute byte controls 2 tiles
    // Unpack: value = (bottomright << 6) | (bottomleft << 4) |
    //                 (topright << 2)    | (topleft << 0)
    const int x2 = x / 2;
    const int y2 = y / 2;
    const uint8_t shift = 2u * (x2 % 2u) + 4u * (y2 % 2u);
    const uint8_t attr_shifted = attribute_raw >> shift;
    const uint8_t palette = attr_shifted & 0b11u;

    return {.attribute = attribute_raw, .palette = palette, .address = address};
}

PpuHelper::Sprite PpuHelper::get_sprite(uint8_t sprite_index) {
    constexpr uint16_t kOamAddr = 0x2003;
    constexpr uint16_t kOamData = 0x2004;
    const uint8_t oam_addr = sprite_index * 4u;

    nes_->ppu().write_byte(kOamAddr, oam_addr);
    const uint8_t y = nes_->ppu().read_byte(kOamData);
    nes_->ppu().write_byte(kOamAddr, oam_addr + 1u);
    const uint8_t tile_index = nes_->ppu().read_byte(kOamData);
    nes_->ppu().write_byte(kOamAddr, oam_addr + 2u);
    const uint8_t attribute = nes_->ppu().read_byte(kOamData);
    nes_->ppu().write_byte(kOamAddr, oam_addr + 3u);
    const uint8_t x = nes_->ppu().read_byte(kOamData);

    const bool flip_vertically = attribute & 0b1000'0000u;
    const bool flip_horizontally = attribute & 0b0100'0000u;
    const bool front_of_bkg = attribute & 0b0010'0000u;
    const uint8_t palette = attribute & 0b0000'0011u;

    return {.x = x,
            .y = y,
            .tile_index = tile_index,
            .attribute = attribute,
            .palette = palette,
            .front_of_bkg = front_of_bkg,
            .flip_horizontally = flip_horizontally,
            .flip_vertically = flip_vertically};
}

sf::Color PpuHelper::get_color_from_index(uint16_t index) {
    return kPalette[index];
}

sf::Color PpuHelper::get_background_color(uint16_t palette, uint8_t index) {
    const uint16_t color_address = 0x3F00 + palette * 4 + index;
    const uint8_t color_index = nes_->ppu_mmu().read_byte(color_address);
    return get_color_from_index(color_index);
}

sf::Color PpuHelper::get_sprite_color(uint16_t palette, uint8_t index) {
    const uint16_t color_address = 0x3F10 + palette * 4 + index;
    const uint8_t color_index = nes_->ppu_mmu().read_byte(color_address);
    return get_color_from_index(color_index);
}

} // namespace nesvis
