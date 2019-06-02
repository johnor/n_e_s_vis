#include "nes.h"
#include "nes_widget.h"
#include "screen.h"

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <stdexcept>

namespace {

constexpr int NES_WIDTH = 256;
constexpr int NES_HEIGHT = 240;
constexpr int PIXEL_SIZE = 4;

constexpr int WIN_WIDTH = NES_WIDTH * PIXEL_SIZE;
constexpr int WIN_HEIGHT = NES_HEIGHT * PIXEL_SIZE;

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
            sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "n_e_s visulization");
    window.setVerticalSyncEnabled(true);

    ImGui::SFML::Init(window);

    Screen screen(NES_WIDTH, NES_HEIGHT, PIXEL_SIZE);
    Nes nes;
    NesWidget nes_widget(&nes);

    for (int i = 0; i < 250; ++i) {
        screen.set_pixel(i, i, 255, 0, 0);
    }

    for (int i = 0; i < 256; ++i) {
        screen.set_pixel(i, 230, 0, 255, 0);
    }
    for (int i = 0; i < 240; ++i) {
        screen.set_pixel(250, i, 0, 0, 255);
    }
    screen.set_pixel(NES_WIDTH - 1, NES_HEIGHT - 1, 0xFFFFFFFF);

    try {
        if (argc > 1) {
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
                pos.x /= PIXEL_SIZE;
                pos.y /= PIXEL_SIZE;

                screen.set_pixel(std::lround(pos.x), std::lround(pos.y), 0, 0, 255);
            }

            ImGui::ShowDemoWindow();
            nes_widget.draw();
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
