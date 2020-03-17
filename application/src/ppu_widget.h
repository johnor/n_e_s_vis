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

class PpuWidget {
public:
    PpuWidget(n_e_s::nes::Nes *nes, PpuHelper *ppu_helper);

    void load_pattern_tables();
    sf::Sprite get_pattern_table_sprite(const int tile_index,
            const int pattern_table);

    void draw();

private:
    uint8_t try_get_ppu_mem(const uint16_t addr);
    n_e_s::nes::Nes *nes_{nullptr};
    PpuHelper *ppu_helper_{nullptr};

    static constexpr int kPatternTableSize{256};
    std::array<sf::Texture, kPatternTableSize * 2> pattern_table_textures_;
    std::array<sf::Sprite, kPatternTableSize * 2> pattern_table_sprites_;
};

} // namespace nesvis
