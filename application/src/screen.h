#pragma once

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cstdint>

namespace nesvis {

class Screen {
public:
    Screen(unsigned int width, unsigned int height, float scaling);

    void draw(sf::RenderWindow &window);

    void set_pixel(unsigned int x,
            unsigned int y,
            uint8_t r,
            uint8_t g,
            uint8_t b);
    void set_pixel(unsigned int x, unsigned int y, uint32_t color);
    void set_pixel(unsigned int x, unsigned int y, const sf::Color &color);

private:
    float scaling_;

    sf::Texture texture_;
    sf::Image image_;
};

} // namespace nesvis
