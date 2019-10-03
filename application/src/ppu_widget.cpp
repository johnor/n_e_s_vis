#include "ppu_widget.h"

#include "core/invalid_address.h"

#include "imgui-SFML.h"
#include "imgui.h"

PpuWidget::PpuWidget(Nes *nes) : nes_{nes} {}

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
