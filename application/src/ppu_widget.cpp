#include "ppu_widget.h"

#include "core/immu.h"
#include "core/invalid_address.h"
#include "core/ippu.h"
#include "nes/nes.h"
#include "ppu_helper.h"

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace nesvis {

PpuWidget::PpuWidget(n_e_s::nes::Nes *nes, PpuHelper *ppu_helper)
        : nes_{nes}, ppu_helper_{ppu_helper} {}

void PpuWidget::load_pattern_tables() {
    const uint16_t palette = 1; // TODO(jn) update to correct index
    for (int i = 0; i < kPatternTableSize; ++i) {
        pattern_table_textures_[i] =
                ppu_helper_->get_pattern_table_texture(i * 16, 0, palette);

        sf::Sprite sprite(pattern_table_textures_[i]);
        sprite.setScale(2.f, 2.f);

        pattern_table_sprites_[i] = sprite;
    }

    for (int i = 0; i < kPatternTableSize; ++i) {
        pattern_table_textures_[kPatternTableSize + i] =
                ppu_helper_->get_pattern_table_texture(i * 16, 1, palette);

        sf::Sprite sprite(pattern_table_textures_[kPatternTableSize + i]);
        sprite.setScale(2.f, 2.f);

        pattern_table_sprites_[kPatternTableSize + i] = sprite;
    }
}

sf::Sprite PpuWidget::get_pattern_table_sprite(const int tile_index,
        const int pattern_table) {
    const int modified_index = tile_index + kPatternTableSize * pattern_table;
    return pattern_table_sprites_[modified_index];
}

void PpuWidget::draw() {
    ImGui::Begin("Nes ppu");

    const auto reg = nes_->ppu_registers();

    ImGui::Text("%s: %02hhX", "ctrl", reg.ctrl);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhX", "mask", reg.mask);

    ImGui::Text("%s: %02hhX", "status", reg.status);

    ImGui::Text("%s: %02hhX", "oamaddr", reg.oamaddr);

    ImGui::Text("%s: %02hhX", "scroll", reg.fine_x_scroll);

    ImGui::Text("%s: %04hX", "vram_addr", reg.vram_addr);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhX", "mem(vram_addr)", try_get_ppu_mem(reg.vram_addr));

    if (ImGui::CollapsingHeader("Pattern tables")) {
        if (ImGui::Button("Read data")) {
            load_pattern_tables();
        }

        ImGui::Text("Pattern table 0");
        for (int i = 0; i < kPatternTableSize; ++i) {
            if (i % 16 != 0) {
                ImGui::SameLine();
            }
            ImGui::Image(pattern_table_sprites_[i]);
        }
        ImGui::Text("Pattern table 1");
        for (int i = kPatternTableSize; i < kPatternTableSize * 2; ++i) {
            if (i % 16 != 0) {
                ImGui::SameLine();
            }
            ImGui::Image(pattern_table_sprites_[i]);
        }
    }

    if (ImGui::CollapsingHeader("Nametables")) {
        const int pattern_table = reg.ctrl & 0x10u ? 1 : 0;
        ImGui::Text("%s: %i", "Pattern table used: ", pattern_table);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        for (uint16_t y = 0; y < 30; ++y) {
            for (uint16_t x = 0; x < 32; ++x) {
                const auto cell_data =
                        ppu_helper_->get_cell_data_from_nametable(x, y);
                const int modified_tile_index =
                        cell_data.tile_index +
                        kPatternTableSize * pattern_table;

                if (x != 0) {
                    ImGui::SameLine();
                }
                ImGui::Image(pattern_table_sprites_[modified_tile_index]);
                if (ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("Address: %04hX", cell_data.address);
                    ImGui::Text("Tile index: %02hX", cell_data.tile_index);
                    ImGui::Text("Tile index mod: %04X", modified_tile_index);
                    ImGui::EndTooltip();
                }
            }
        }
        ImGui::PopStyleVar(1);
    }

    ImGui::End();
}

uint8_t PpuWidget::try_get_ppu_mem(const uint16_t addr) {
    uint8_t result = 0u;
    try {
        result = nes_->ppu().read_byte(addr);
    } catch (const n_e_s::core::InvalidAddress &) {
    }
    return result;
}

} // namespace nesvis
