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

    ImGui::SameLine();
    ImGui::Text("Curr cycle: %i", nes_->cycle());

    ImGui::Text("%s: %04hx", "pc", nes_->cpu_registers_.pc);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "a", nes_->cpu_registers_.a);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "x", nes_->cpu_registers_.x);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "y", nes_->cpu_registers_.y);
    ImGui::SameLine();
    ImGui::Text("%s: %02hhx", "sp", nes_->cpu_registers_.sp);

    ImGui::End();
}