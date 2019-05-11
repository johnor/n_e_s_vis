#include "nes.h"
#include "nes_widget.h"

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <stdexcept>

int main(int argc, char **argv) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "n_e_s visulization");
    window.setVerticalSyncEnabled(true);
    ImGui::SFML::Init(window);

    Nes nes;
    NesWidget nesWidget(&nes);

    try {
        nes.load_rom(argv[1]);

        sf::Clock delta_clock;
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);

                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            const sf::Time delta_time = delta_clock.restart();
            ImGui::SFML::Update(window, delta_time);

            ImGui::ShowDemoWindow();

            nesWidget.draw();

            window.clear();
            ImGui::SFML::Render(window);
            window.display();
        }

        ImGui::SFML::Shutdown();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
