#pragma once

#include "imgui-SFML.h"
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "nes/core/immu.h"
#include "nes/core/invalid_address.h"
#include "nes/nes.h"

#include <fmt/format.h>
#include <functional>
#include <map>
#include <optional>
#include <string>

namespace nesvis {

class MemoryWidget {
public:
    explicit MemoryWidget(n_e_s::nes::Nes *nes) : nes_(nes) {}

    void draw() {
        ImGui::Begin("Memory viewer", nullptr);

        ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None);

        if (ImGui::BeginTabItem("Cpu")) {
            draw_tab("Cpu",
                    cpu_mem_labels_,
                    0xFFFF,
                    [this](uint16_t address) -> uint8_t {
                        return nes_->mmu().read_byte(address);
                    });
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Ppu")) {
            draw_tab("Ppu",
                    ppu_mem_lables_,
                    0x4000,
                    [this](uint16_t address) -> uint8_t {
                        return nes_->ppu_mmu().read_byte(address);
                    });
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
        ImGui::End();
    }

private:
    void draw_tab(const std::string &title,
            const std::map<uint16_t, std::string> &memory_labels,
            uint16_t memory_size,
            const std::function<uint8_t(uint16_t)> &memory_read_func) {
        if (memory_labels.size() > 0) {
            ImGui::Button((title + " Map").c_str());
            if (ImGui::BeginPopupContextItem(
                        (title + "jump_context_menu").c_str(), 0)) {
                for (auto const &[address, address_str] : memory_labels) {
                    const std::string label =
                            fmt::format("${:04X}: {}", address, address_str);
                    if (ImGui::MenuItem(label.c_str())) {
                        goto_address_ = address;
                    }
                }
                ImGui::EndPopup();
            }
        }

        constexpr int kColsPerLine = 16;
        if (ImGui::BeginChild((title + "memory_child").c_str(), {0, 400})) {
            const int line_count_total =
                    ((memory_size + kColsPerLine - 1) / kColsPerLine);
            ImGuiListClipper clipper(line_count_total);
            while (clipper.Step()) {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd;
                        ++row) {
                    ImGui::Text("%04X | ", row * kColsPerLine);
                    for (int col = 0; col < kColsPerLine; col++) {
                        const uint16_t curr_address =
                                (row * kColsPerLine) + col;

                        const auto mem_value =
                                read_mem(memory_read_func, curr_address);

                        ImGui::SameLine();
                        draw_mem_value(curr_address, mem_value);
                    }
                }
            }

            if (goto_address_) {
                const auto target_index = goto_address_.value() / kColsPerLine;
                const float adjusted_offset =
                        ImGui::GetCursorStartPos().y +
                        target_index * ImGui::GetTextLineHeightWithSpacing();
                ImGui::SetScrollFromPosY(adjusted_offset, 0.f);
                goto_address_ = std::nullopt;
            }
        }
        ImGui::EndChild();

        ImGui::Separator();

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Address:");
        ImGui::SameLine();
        ImGui::PushItemWidth(60.f);
        if (ImGui::InputText("##addr",
                    &goto_address_input_,
                    ImGuiInputTextFlags_CharsHexadecimal |
                            ImGuiInputTextFlags_EnterReturnsTrue)) {
            goto_address_ = std::stoi(goto_address_input_, nullptr, 16);
        }
        ImGui::PopItemWidth();
    }

    std::optional<uint8_t> read_mem(
            const std::function<uint8_t(uint16_t)> &memory_read_func,
            const uint16_t address) {
        try {
            return memory_read_func(address);
        } catch (n_e_s::core::InvalidAddress &) {
            return std::nullopt;
        }
    }

    void draw_mem_value(uint16_t address,
            const std::optional<uint8_t> &mem_value) {
        if (mem_value) {
            if (mem_value.value() == 0u) {
                ImGui::TextDisabled("00");
            } else {
                ImGui::Text("%02hhX", mem_value.value());
            }
        } else {
            ImGui::TextDisabled(" -");
        }

        if (ImGui::IsItemHovered()) {
            goto_address_input_ = fmt::format("{:04X}", address);

            ImVec2 pos = ImGui::GetItemRectMin();
            pos.x -= 4;
            pos.y -= 4;
            const auto color = IM_COL32(150u, 150u, 150u, 100u);
            ImGui::GetWindowDrawList()->AddRectFilled(
                    pos, ImVec2(pos.x + 24, pos.y + 24), color);

            ImGui::BeginTooltip();
            ImGui::Text("%04hX", address);
            ImGui::EndTooltip();
        }
    }

    n_e_s::nes::Nes *nes_{nullptr};
    std::string goto_address_input_;
    std::optional<uint16_t> goto_address_;

    const std::map<uint16_t, std::string> cpu_mem_labels_ = {{0x0000, "Ram"},
            {0x0800, "Mirror of 0x0000-0x07FF"},
            {0x1000, "Mirror of 0x0000-0x07FF"},
            {0x1800, "Mirror of 0x0000-0x07FF"},
            {0x2000, "PPU Registers"},
            {0x2008, "Mirror of 0x2000-0x2007"},
            {0x4000, "Apu and I/O register"},
            {0x4020, "Cartridge space"}};
    const std::map<uint16_t, std::string> ppu_mem_lables_ = {
            {0x0000, "Pattern table 0"},
            {0x1000, "Pattern table 1"},
            {0x2000, "Nametable 0"},
            {0x2400, "Nametable 1"},
            {0x2800, "Nametable 2"},
            {0x2C00, "Nametable 3"},
            {0x3000, "Mirror of 0x2000-0x2EFF"},
            {0x3F00, "Palette"},
            {0x3F20, "Mirror of 0x3F00-0x3F1F"},
    };
};

} // namespace nesvis
