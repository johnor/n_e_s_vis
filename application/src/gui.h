#pragma once

#include "cpu_widget.h"
#include "imgui.h"
#include "log_widget.h"
#include "memory_widget.h"
#include "ppu_helper.h"
#include "ppu_widget.h"

#include "imgui_internal.h"

namespace nesvis {

class Control;

class Gui {
public:
    Gui(n_e_s::nes::Nes *nes, PpuHelper *ppu_helper, Control *control)
            : log_widget(),
              cpu_widget(nes, control),
              ppu_widget(nes, ppu_helper),
              memory_widget(nes) {}

    void draw() {
        if (first_) {
            const ImVec2 viewport_pos = ImGui::GetMainViewport()->Pos;
            const ImVec2 viewport_size = ImGui::GetMainViewport()->Size;

            const ImGuiID dock_id_right =
                    ImGui::DockBuilderAddNode(0, ImGuiDockNodeFlags_None);
            ImGui::DockBuilderSetNodePos(dock_id_right,
                    ImVec2(viewport_size.x - 500, viewport_pos.y));
            ImGui::DockBuilderSetNodeSize(
                    dock_id_right, ImVec2(500, viewport_size.y));
            ImGui::DockBuilderDockWindow("Nes cpu", dock_id_right);
            ImGui::DockBuilderDockWindow("Nes ppu", dock_id_right);
            ImGui::DockBuilderDockWindow("Memory viewer", dock_id_right);
            ImGui::DockBuilderFinish(dock_id_right);

            first_ = false;
        }

        cpu_widget.draw();
        ppu_widget.draw();
        memory_widget.draw();
        log_widget.draw();
    }

    nesvis::LogWidget log_widget;
    nesvis::CpuWidget cpu_widget;
    nesvis::PpuWidget ppu_widget;
    nesvis::MemoryWidget memory_widget;

    bool first_{true};
};

} // namespace nesvis
