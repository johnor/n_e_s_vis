#include "log_widget.h"

#include "imgui-SFML.h"
#include "imgui.h"

namespace nesvis {

void LogWidget::draw() {
    const ImVec2 viewport_size = ImGui::GetMainViewport()->Size;

    ImGui::Begin("Log");
    ImGui::SetWindowPos(
            ImVec2(0, viewport_size.y - 200), ImGuiCond_FirstUseEver);
    ImGui::SetWindowSize(
            ImVec2(viewport_size.x - 500, 200), ImGuiCond_FirstUseEver);

    ImGui::BeginChild("Scrolling");
    for (const auto &a : text_list_) {
        ImGui::Text("%s", a.c_str());
    }
    ImGui::EndChild();

    ImGui::End();
}

void LogWidget::add(const std::string &text) {
    text_list_.push_back(text);
    if (text_list_.size() > 100u) {
        text_list_.pop_back();
    }
}

} // namespace nesvis
