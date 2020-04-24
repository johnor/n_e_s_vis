#include "ppu_widget.h"

#include "nes/core/immu.h"
#include "nes/core/invalid_address.h"
#include "nes/core/ippu.h"
#include "nes/nes.h"
#include "ppu_helper.h"

#include "imconfig-SFML.h"
#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/Texture.hpp>

namespace nesvis {

PpuWidget::PpuWidget(n_e_s::nes::Nes *nes, PpuHelper *ppu_helper)
        : nes_{nes}, ppu_helper_{ppu_helper} {}

void PpuWidget::load_pattern_tables() {
    const uint16_t palette = 0; // TODO(jn) update to correct index
    for (int i = 0; i < kPatternTableSize; ++i) {
        pattern_table_textures_[i] =
                ppu_helper_->get_pattern_table_texture(i, 0, palette);
    }

    for (int i = 0; i < kPatternTableSize; ++i) {
        pattern_table_textures_[kPatternTableSize + i] =
                ppu_helper_->get_pattern_table_texture(i, 1, palette);
    }
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
        draw_patterntables();
    }
    if (ImGui::CollapsingHeader("Nametables")) {
        draw_nametables();
    }
    if (ImGui::CollapsingHeader("Sprites")) {
        draw_sprites();
    }
    if (ImGui::CollapsingHeader("Palettes")) {
        draw_palettes();
    }

    ImGui::End(); // window
}
void PpuWidget::draw_patterntables() {
    if (ImGui::Button("Read data")) {
        load_pattern_tables();
    }

    ImGui::Text("Pattern table 0");
    for (int i = 0; i < kPatternTableSize; ++i) {
        if (i % 16 != 0) {
            ImGui::SameLine();
        }
        ImGui::Image(pattern_table_textures_[i], {kSpriteSize, kSpriteSize});
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Index: %02X", i);
            ImGui::EndTooltip();
        }
    }
    ImGui::Text("Pattern table 1");
    for (int i = kPatternTableSize; i < kPatternTableSize * 2; ++i) {
        if (i % 16 != 0) {
            ImGui::SameLine();
        }
        ImGui::Image(pattern_table_textures_[i], {kSpriteSize, kSpriteSize});
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Index: %02X", i - kPatternTableSize);
            ImGui::EndTooltip();
        }
    }
}

void PpuWidget::draw_nametables() {
    const auto reg = nes_->ppu_registers();
    const int pattern_table = reg.ctrl & 0x10u ? 1 : 0;
    ImGui::Text("%s: %i", "Pattern table used: ", pattern_table);

    if (ImGui::BeginTabBar("MyTabBar")) {
        for (auto nametable = 0; nametable < 4; ++nametable) {
            const std::string tab_name = "Nametable " + std::to_string(nametable);
            if (ImGui::BeginTabItem(tab_name.c_str())) {
                draw_nametable(nametable);
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
}
void PpuWidget::draw_nametable(uint16_t nametable) {
    const auto reg = nes_->ppu_registers();
    const int pattern_table = reg.ctrl & 0x10u ? 1 : 0;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    for (uint16_t y = 0; y < 30; ++y) {
        for (uint16_t x = 0; x < 32; ++x) {
            const auto nametable_cell =
                    ppu_helper_->get_nametable_cell(x, y, nametable);
            const auto attr_cell = ppu_helper_->get_attribute_cell(x, y);
            const int modified_tile_index = nametable_cell.tile_index +
                                            kPatternTableSize * pattern_table;

            if (x != 0) {
                ImGui::SameLine();
            }
            ImGui::Image(pattern_table_textures_[modified_tile_index],
                    {kSpriteSize, kSpriteSize});
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("Ppu address: %04hX", nametable_cell.address);
                ImGui::Text("Tile index: %02hX", nametable_cell.tile_index);
                ImGui::Text("Tile index mod: %04X", modified_tile_index);
                ImGui::Text("Attr address: %04hX", attr_cell.address);
                ImGui::Text("Attribute: %02hX", attr_cell.attribute);
                ImGui::Text("Palette: %02hhX", attr_cell.palette);
                ImGui::EndTooltip();
            }
        }
    }
    ImGui::PopStyleVar(1);
}

void PpuWidget::draw_sprites() {
    const auto reg = nes_->ppu_registers();
    const int sprite_pattern_table = reg.ctrl & 0b000'1000u ? 1 : 0;
    ImGui::Text("%s: %i", "Sprite pattern table: ", sprite_pattern_table);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    for (uint8_t index = 0; index < 64; ++index) {
        const auto sprite = ppu_helper_->get_sprite(index);
        const int modified_tile_index =
                sprite.tile_index + kPatternTableSize * sprite_pattern_table;

        if (index % 8 != 0) {
            ImGui::SameLine();
        }
        ImGui::Image(pattern_table_textures_[modified_tile_index],
                {kSpriteSize, kSpriteSize});
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Index: %02hhX", index);
            ImGui::Text("x: %hhu", sprite.x);
            ImGui::Text("y: %hhu", sprite.y);
            ImGui::Text("Palette: %02hhX", sprite.palette);
            ImGui::EndTooltip();
        }
    }
    ImGui::PopStyleVar(1);
}

void PpuWidget::draw_palettes() {
    ImGui::Text("Background palette");
    ImGui::PushID("background palette");

    const std::array<std::string, 8> palette_text = {"Background 0",
            "Background 1",
            "Background 2",
            "Background 3",
            "Sprite 0",
            "Sprite 1",
            "Sprite 2",
            "Sprite 3"};
    int text_index = 0;

    for (uint16_t address = 0x3F00; address < 0x3F20; ++address) {
        const uint8_t palette_index = nes_->ppu().read_byte(address);
        ImGui::PushID(palette_index);
        if ((address % 4) == 0) {
            ImGui::Text("%s", palette_text[text_index++].c_str());
            ImGui::SameLine(100);
        }
        if ((address % 4) != 0) {
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);
        }
        const ImVec4 color = ppu_helper_->get_color_from_index(palette_index);
        ImGui::ColorButton("##backgroundpalette",
                color,
                ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip,
                ImVec2(20, 20));
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::Text("Address: %04hX", address);
            ImGui::Text("index: %02hhX", palette_index);
            ImGui::EndTooltip();
        }

        ImGui::PopID();
    }
    ImGui::PopID(); // background palette

    ImGui::PushID("palette");
    for (int n = 0; n < ppu_helper_->kPaletteSize; n++) {
        ImGui::PushID(n);
        if ((n % 16) != 0) {
            ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);
        }
        const ImVec4 color = ppu_helper_->get_color_from_index(n);
        ImGui::ColorButton("##palette",
                color,
                ImGuiColorEditFlags_NoPicker,
                ImVec2(20, 20));

        ImGui::PopID();
    }
    ImGui::PopID(); // palette
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
