#include "control.h"
#include "cpu_widget.h"
#include "gui.h"
#include "screen.h"
#include "simple_renderer.h"

#include "nes/nes.h"

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <fmt/format.h>

#include <cmath>
#include <fstream>
#include <stdexcept>

namespace {

constexpr int kNesWidth = 256;
constexpr int kNesHeight = 240;
constexpr float kNesScaleFactor = 3.f;

constexpr int kWinWidth = kNesWidth * 5;
constexpr int kWinHeight = kNesHeight * 4;

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
    constexpr int kFps = 20;
    window.setFramerateLimit(kFps);

    ImGui::SFML::Init(window);
    ImGuiIO &io = ImGui::GetIO();
    // NOLINTNEXTLINE(hicpp-signed-bitwise)
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingNoSplit = true;

    nesvis::Screen screen(kNesWidth, kNesHeight, kNesScaleFactor);
    n_e_s::nes::Nes nes;
    nesvis::Control control(&nes);
    nesvis::PpuHelper ppu_helper(&nes);
    nesvis::Gui gui(&nes, &ppu_helper, &control);

    nesvis::SimpleRenderer renderer(&nes, &ppu_helper, &screen);

    try {
        if (argc > 1) {
            gui.log_widget.add("Loading rom from: " + std::string(argv[1]));
            fmt::print("Loading rom from: {}\n", argv[1]);
            std::ifstream fs(argv[1], std::ios::binary);
            if (!fs) {
                fmt::print(stderr, "Could not load file: {}\n", argv[1]);
                return 1;
            }
            nes.load_rom(fs);
        }

        gui.ppu_widget.load_pattern_tables();

        sf::Clock delta_clock;
        while (window.isOpen()) {
            poll_events(window);

            const sf::Time delta_time = delta_clock.restart();
            ImGui::SFML::Update(window, delta_time);

            window.clear();

            renderer.draw();
            gui.draw();
            screen.draw(window);

            ImGui::SFML::Render(window);
            window.display();

            // robinlinden/desunes
            constexpr size_t kMasterClock = 21'477'272; // Hz
            // Tick rates:
            // CPU: every 12 master ticks
            // PPU: every 4
            // APU: every 24
            constexpr size_t kNESClock = kMasterClock;
            constexpr size_t kTickPerFrame = kNESClock / kFps;
            if (control.is_running()) {
                for (size_t i = 0; i < kTickPerFrame; ++i) {
                    nes.execute();
                }
            }

            const auto frame_time = delta_time;
            if (frame_time.asMilliseconds() > 1000 / kFps) {
                fmt::print(stderr,
                        "Things are running slowly: {}ms\n",
                        frame_time.asMilliseconds());
            }
        }

        ImGui::SFML::Shutdown();
    } catch (const std::exception &e) {
        fmt::print(stderr, "Caught exception: {}\n", e.what());
    }

    return 0;
}
