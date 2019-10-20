#include "cpu_widget.h"

#include "core/icpu.h"
#include "core/immu.h"
#include "core/opcode.h"
#include "nes/nes.h"

#include "imgui-SFML.h"
#include "imgui.h"

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

    ImGui::Text("Curr cycle: %lu", nes_->current_cycle());

    const auto reg = nes_->cpu_registers();

    ImGui::Text("%s: %04hx", "pc", reg.pc);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "a", reg.a);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "x", reg.x);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "y", reg.y);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "p", reg.p);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "sp", reg.sp);

    ImGui::Spacing();

    const uint16_t pc = reg.pc;
    ImGui::Text("Mem at PC: %02hhx %02hhx %02hhx",
            nes_->mmu().read_byte(pc),
            nes_->mmu().read_byte(pc + 1),
            nes_->mmu().read_byte(pc + 2));

    const auto opcode = n_e_s::core::decode(nes_->mmu().read_byte(pc));
    ImGui::Text(
            "Instruction: %s", n_e_s::core::to_string(opcode.family).c_str());

    ImGui::End();
}
