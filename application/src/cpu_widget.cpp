#include "cpu_widget.h"

#include "core/immu.h"
#include "core/imos6502.h"
#include "core/opcode.h"
#include "nes/nes.h"

#include "imgui-SFML.h"
#include "imgui.h"

namespace nesvis {

CpuWidget::CpuWidget(n_e_s::nes::Nes *nes) : nes_{nes} {}

void CpuWidget::draw() {
    ImGui::Begin("Nes cpu");

    if (ImGui::Button("Step")) {
        nes_->execute();
    }
    ImGui::SameLine();
    if (ImGui::Button("Step 1000")) {
        for (int i = 0; i < 1000; ++i) {
            nes_->execute();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Step 10000")) {
        for (int i = 0; i < 10000; ++i) {
            nes_->execute();
        }
    }

    if (ImGui::Button("Jump to address")) {
        nes_->reset();
        nes_->cpu_registers().pc = jump_to_address_;
    }
    ImGui::SameLine();
    ImGui::InputScalar("",
            ImGuiDataType_U16,
            &jump_to_address_,
            nullptr,
            nullptr,
            "%04hx",
            ImGuiInputTextFlags_CharsHexadecimal);

    if (ImGui::Button("Reset")) {
        nes_->reset();
    }

    ImGui::Text("Curr cycle: %llu", nes_->current_cycle());

    const auto reg = nes_->cpu_registers();

    ImGui::Text("%s: %04hX", "pc", reg.pc);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhX", "a", reg.a);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhX", "x", reg.x);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhX", "y", reg.y);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhX", "p", reg.p);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhX", "sp", reg.sp);

    ImGui::Spacing();

    const uint16_t pc_start = nes_->cpu().state().start_pc;
    ImGui::Text("Mem at PC: %02hhX %02hhX %02hhX",
            nes_->mmu().read_byte(pc_start),
            nes_->mmu().read_byte(pc_start + 1),
            nes_->mmu().read_byte(pc_start + 2));

    const auto opcode = n_e_s::core::decode(nes_->mmu().read_byte(pc_start));
    ImGui::Text(
            "Instruction: %s", n_e_s::core::to_string(opcode.family).c_str());

    ImGui::Text("Stack contents:");
    for (int i = reg.sp + 1; i <= 0xFF; ++i) {
        ImGui::Text("%02X: %02hhX", i, nes_->mmu().read_byte(0x0100 + i));
    }

    ImGui::End();
}

} // namespace nesvis
