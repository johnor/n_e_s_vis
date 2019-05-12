#include "nes_widget.h"

#include "imgui-SFML.h"
#include "imgui.h"

NesWidget::NesWidget(Nes *nes) : nes_{nes} {}

void NesWidget::draw() {
    ImGui::Begin("Nes");

    if (ImGui::Button("Step")) {
        nes_->execute();
    }
    ImGui::SameLine();
    if (ImGui::Button("Step 1000")) {
        for (int i = 0; i < 1000; ++i) {
            nes_->execute();
        }
    }

    if (ImGui::Button("Jump to address")) {
        nes_->reset();
        nes_->cpu_registers_.pc = jump_to_address_;
    }
    ImGui::SameLine();
    ImGui::InputScalar("",
            ImGuiDataType_U16,
            &jump_to_address_,
            nullptr,
            nullptr,
            "%04hx",
            ImGuiInputTextFlags_CharsHexadecimal);

    ImGui::Text("Curr cycle: %lu", nes_->cycle());

    ImGui::Text("%s: %04hx", "pc", nes_->cpu_registers_.pc);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "a", nes_->cpu_registers_.a);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "x", nes_->cpu_registers_.x);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "y", nes_->cpu_registers_.y);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "p", nes_->cpu_registers_.p);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "sp", nes_->cpu_registers_.sp);

    ImGui::Spacing();

    const uint16_t pc = nes_->cpu_registers_.pc;
    ImGui::Text("Mem at PC: %02hhx %02hhx %02hhx",
                    nes_->mmu_->read_byte(pc),
                    nes_->mmu_->read_byte(pc+1),
                    nes_->mmu_->read_byte(pc+2));

    ImGui::End();
}