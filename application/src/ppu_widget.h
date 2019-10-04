#pragma once

#include "nes.h"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <array>

class PpuWidget {
public:
    explicit PpuWidget(Nes *nes);

    void draw();

private:
    uint8_t try_get_ppu_mem(const uint16_t addr);
    Nes *nes_{nullptr};

    sf::Texture get_pattern_table(uint16_t pos, uint16_t pattern_table);

    static constexpr int kPatternTableSize{256};
    std::array<sf::Texture, kPatternTableSize * 2> pattern_table_textures_;
    std::array<sf::Sprite, kPatternTableSize * 2> pattern_table_sprites_;
};