#include "ppu_widget.h"

#include "core/invalid_address.h"
#include "core/immu.h"
#include "core/ippu.h"
#include "nes/nes.h"

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

PpuWidget::PpuWidget(n_e_s::nes::Nes *nes) : nes_{nes} {}

void PpuWidget::draw() {
    ImGui::Begin("Nes ppu");

    const auto reg = nes_->ppu_registers();

    ImGui::Text("%s: %02hhx", "ctrl", reg.ctrl);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "mask", reg.mask);

    ImGui::Text("%s: %02hhx", "status", reg.status);

    ImGui::Text("%s: %02hhx", "oamaddr", reg.oamaddr);

    ImGui::Text("%s: %02hhx", "scroll", reg.fine_x_scroll);

    ImGui::Text("%s: %04hx", "vram_addr", reg.vram_addr);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "mem(vram_addr)", try_get_ppu_mem(reg.vram_addr));

    if (ImGui::CollapsingHeader("Pattern tables")) {
        if (ImGui::Button("Read data")) {
            for (int i = 0; i < kPatternTableSize; ++i) {
                pattern_table_textures_[i] = get_pattern_table(i * 16, 0);

                sf::Sprite sprite(pattern_table_textures_[i]);
                sprite.setScale(2.f, 2.f);

                pattern_table_sprites_[i] = sprite;
            }

            for (int i = 0; i < kPatternTableSize; ++i) {
                pattern_table_textures_[kPatternTableSize + i] =
                        get_pattern_table(i * 16, 1);

                sf::Sprite sprite(pattern_table_textures_[kPatternTableSize + i]);
                sprite.setScale(2.f, 2.f);

                pattern_table_sprites_[kPatternTableSize + i] = sprite;
            }
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

sf::Texture PpuWidget::get_pattern_table(uint16_t pos, uint16_t pattern_table) {
    sf::Image image;
    image.create(8, 8, sf::Color(10, 100, 0));

    for (uint8_t row = 0; row < 8; ++row) {
        // Second pattern table starts at 0x1000
        const uint16_t base_address = pattern_table * 0x1000 + pos + row;
        const uint8_t a = nes_->ppu_mmu().read_byte(base_address);
        const uint8_t b = nes_->ppu_mmu().read_byte(base_address + 8u);

        for (uint8_t col = 0; col < 8u; ++col) {
            // First column is the leftmost bit
            const uint16_t mask = 1u << (7 - col);

            const uint8_t color_index = !!(a & mask) + !!(b & mask);

            sf::Color color = sf::Color::Black;
            switch (color_index) {
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
            image.setPixel(col, row, color);
        }
    }
    sf::Texture texture;
    texture.loadFromImage(image);
    return texture;
}
