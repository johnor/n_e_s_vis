#include "simple_renderer.h"

#include "core/ippu.h"
#include "nes/nes.h"
#include "ppu_helper.h"
#include "screen.h"

namespace nesvis {

SimpleRenderer::SimpleRenderer(n_e_s::nes::Nes *nes,
        PpuHelper *ppu_helper,
        Screen *screen)
        : nes_{nes}, ppu_helper_{ppu_helper}, screen_{screen} {}

void SimpleRenderer::draw() {
    const int pattern_table = nes_->ppu_registers().ctrl & 0x10u ? 1 : 0;

    for (uint16_t cell_y = 0; cell_y < 30; ++cell_y) {
        for (uint16_t cell_x = 0; cell_x < 32; ++cell_x) {
            const auto nametable_cell =
                    ppu_helper_->get_nametable_cell(cell_x, cell_y);
            const auto attr_cell =
                    ppu_helper_->get_attribute_cell(cell_x, cell_y);

            const auto pattern_cell = ppu_helper_->get_pattern_table_cell(
                    nametable_cell.tile_index, pattern_table);

            for (uint16_t pattern_y = 0; pattern_y < 8; ++pattern_y) {
                for (uint16_t pattern_x = 0; pattern_x < 8; ++pattern_x) {
                    const int data_index = pattern_y * 8 + pattern_x;
                    const int index = pattern_cell.data[data_index];

                    const auto color = ppu_helper_->get_background_color(
                            attr_cell.palette, index);
                    const int pixel_x = cell_x * 8 + pattern_x;
                    const int pixel_y = cell_y * 8 + pattern_y;
                    screen_->set_pixel(pixel_x, pixel_y, color);
                }
            }
        }
    }
}

} // namespace nesvis
