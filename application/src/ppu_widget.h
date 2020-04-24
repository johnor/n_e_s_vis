#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <array>
#include <cstdint>

namespace n_e_s::nes {
class Nes;
} // namespace n_e_s::nes

namespace nesvis {

class PpuHelper;

class PpuWidget {
public:
    PpuWidget(n_e_s::nes::Nes *nes, PpuHelper *ppu_helper);

    void load_pattern_tables();

    void draw();

private:
    void draw_patterntables();
    void draw_nametables();
    void draw_sprites();
    void draw_palettes();
    uint8_t try_get_ppu_mem(const uint16_t addr);

    n_e_s::nes::Nes *nes_{nullptr};
    PpuHelper *ppu_helper_{nullptr};

    static constexpr float kSpriteSize = 16.f;
    static constexpr int kPatternTableSize{256};
    std::array<sf::Texture, kPatternTableSize * 2> pattern_table_textures_;
};

} // namespace nesvis
