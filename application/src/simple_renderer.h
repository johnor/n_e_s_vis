#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <array>
#include <cstdint>

namespace n_e_s::nes {
class Nes;
} // namespace n_e_s::nes

namespace nesvis {

class PpuHelper;
class Screen;

class SimpleRenderer {
public:
    SimpleRenderer(n_e_s::nes::Nes *nes, PpuHelper *ppu_helper);

    void render(Screen *screen);

private:
    n_e_s::nes::Nes *nes_{nullptr};
    PpuHelper *ppu_helper_{nullptr};
};

} // namespace nesvis
