#include "log_widget.h"

#include "imgui-SFML.h"
#include "imgui.h"

namespace nesvis {

LogWidget::LogWidget() {}

void LogWidget::draw() {
    ImGui::Begin("Log");

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