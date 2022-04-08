#include "simple_renderer.h"

#include "nes/core/ippu.h"
#include "nes/nes.h"
#include "ppu_helper.h"
#include "screen.h"

namespace nesvis {

SimpleRenderer::SimpleRenderer(n_e_s::nes::Nes *nes,
        PpuHelper *ppu_helper,
        Screen *screen)
        : nes_{nes}, ppu_helper_{ppu_helper}, screen_{screen} {}

void SimpleRenderer::draw() {
    const int bkg_pattern_table = nes_->ppu_registers().ctrl.is_set(4u) ? 1 : 0;

    for (uint16_t cell_y = 0; cell_y < 30; ++cell_y) {
        for (uint16_t cell_x = 0; cell_x < 32; ++cell_x) {
            // TODO (jn) What to set nametable to?
            const uint16_t nametable = 0;
            const auto nametable_cell =
                    ppu_helper_->get_nametable_cell(cell_x, cell_y, nametable);
            const auto attr_cell =
                    ppu_helper_->get_attribute_cell(cell_x, cell_y);

            const auto pattern_cell = ppu_helper_->get_pattern_table_cell(
                    nametable_cell.tile_index, bkg_pattern_table);

            for (uint16_t pattern_y = 0; pattern_y < 8; ++pattern_y) {
                for (uint16_t pattern_x = 0; pattern_x < 8; ++pattern_x) {
                    const int data_index = pattern_y * 8 + pattern_x;
                    const int color_index = pattern_cell.data[data_index];

                    const auto color = ppu_helper_->get_background_color(
                            attr_cell.palette, color_index);
                    const int pixel_x = cell_x * 8 + pattern_x;
                    const int pixel_y = cell_y * 8 + pattern_y;
                    screen_->set_pixel(pixel_x, pixel_y, color);
                }
            }
        }
    }

    const int sprite_pattern_table =
            nes_->ppu_registers().ctrl.is_set(3u) ? 1 : 0;
    for (uint8_t sprite_index = 0; sprite_index < 64; ++sprite_index) {
        const auto sprite = ppu_helper_->get_sprite(sprite_index);

        const auto pattern_cell = ppu_helper_->get_pattern_table_cell(
                sprite.tile_index, sprite_pattern_table);

        for (uint16_t pattern_y = 0; pattern_y < 8; ++pattern_y) {
            for (uint16_t pattern_x = 0; pattern_x < 8; ++pattern_x) {
                const int flipped_x =
                        sprite.flip_horizontally ? (7 - pattern_x) : pattern_x;
                const int flipped_y =
                        sprite.flip_vertically ? (7 - pattern_y) : pattern_y;
                const int data_index = flipped_y * 8 + flipped_x;
                const int color_index = pattern_cell.data[data_index];

                if (color_index > 0) {
                    const auto color = ppu_helper_->get_sprite_color(
                            sprite.palette, color_index);
                    const int pixel_x = sprite.x + pattern_x;
                    const int pixel_y = sprite.y + pattern_y;
                    screen_->set_pixel(pixel_x, pixel_y, color);
                }
            }
        }
    }
}

} // namespace nesvis
