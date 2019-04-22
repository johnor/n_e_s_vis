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

    sf::Color bgColor;
    float color[3] = {0.f, 0.f, 0.f};

    std::vector<float> times;

    Nes nes;
    NesWidget nesWidget(&nes);

    try {
        nes.load_rom(argv[1]);

        sf::Clock deltaClock;
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);

                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            sf::Time t = deltaClock.restart();
            times.push_back((float)t.asMilliseconds());
            ImGui::SFML::Update(window, t);

            ImGui::ShowDemoWindow();

            nesWidget.draw();

            ImGui::Begin("Sample window");

            if (ImGui::ColorEdit3("Background color", color)) {
                bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
                bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
                bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
            }

            ImGui::End();

            ImGui::Begin("Test");
            {
                ImGui::PlotLines("Frame Times", times.data(), times.size());
                ImGui::PlotLines(
                        "Sin",
                        [](void *data, int idx) { return sinf(idx * 0.2f); },
                        NULL,
                        100);
                // Display contents in a scrolling region
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
                ImGui::BeginChild("Scrolling");
                for (int n = 0; n < 50; n++)
                    ImGui::Text("%04d: Some text", n);
                ImGui::EndChild();
            }
            ImGui::End();

            window.clear(bgColor);
            ImGui::SFML::Render(window);
            window.display();
        }

        ImGui::SFML::Shutdown();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
