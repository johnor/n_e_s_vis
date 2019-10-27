#include "cpu_widget.h"
#include "log_widget.h"
#include "ppu_widget.h"
#include "screen.h"

#include "nes/nes.h"

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace {

constexpr int kNesWidth = 256;
constexpr int kNesHeight = 240;
constexpr int kPixelSize = 4;

constexpr int kWinWidth = kNesWidth * kPixelSize;
constexpr int kWinHeight = kNesHeight * kPixelSize;

void poll_events(sf::RenderWindow &window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

} // namespace

int main(int argc, char **argv) {
    sf::RenderWindow window(
            sf::VideoMode(kWinWidth, kWinHeight), "n_e_s visulization");
    window.setVerticalSyncEnabled(true);

    ImGui::SFML::Init(window);

    nesvis::Screen screen(kNesWidth, kNesHeight, kPixelSize);
    n_e_s::nes::Nes nes;
    nesvis::LogWidget log;
    nesvis::CpuWidget cpu_widget(&nes);
    nesvis::PpuWidget ppu_widget(&nes);

    for (int i = 0; i < 250; ++i) {
        screen.set_pixel(i, i, 255, 0, 0);
    }

    for (int i = 0; i < 256; ++i) {
        screen.set_pixel(i, 230, 0, 255, 0);
    }
    for (int i = 0; i < 240; ++i) {
        screen.set_pixel(250, i, 0, 0, 255);
    }
    screen.set_pixel(kNesWidth - 1, kNesHeight - 1, 0xFFFFFFFF);

    try {
        if (argc > 1) {
            log.add("Loading rom from: " + std::string(argv[1]));
            nes.load_rom(argv[1]);
        }

        sf::Clock delta_clock;
        while (window.isOpen()) {
            poll_events(window);

            const sf::Time delta_time = delta_clock.restart();
            ImGui::SFML::Update(window, delta_time);

            window.clear();

            if (ImGui::IsMouseDown(0)) {
                auto pos = sf::Vector2f(ImGui::GetMousePos());
                pos.x /= kPixelSize;
                pos.y /= kPixelSize;

                screen.set_pixel(
                        std::lround(pos.x), std::lround(pos.y), 0, 0, 255);
            }

            ImGui::ShowDemoWindow();
            cpu_widget.draw();
            ppu_widget.draw();
            log.draw();
            screen.draw(window);

            ImGui::SFML::Render(window);

            window.display();
        }

        ImGui::SFML::Shutdown();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
