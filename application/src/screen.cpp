#include "screen.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace nesvis {

Screen::Screen(unsigned int width, unsigned int height, float scaling)
        : scaling_(scaling), image_() {
    image_.create(width, height, sf::Color(0, 0, 0));
}

void Screen::set_pixel(unsigned int x,
        unsigned int y,
        uint8_t r,
        uint8_t g,
        uint8_t b) {
    set_pixel(x, y, sf::Color(r, g, b));
}

void Screen::set_pixel(unsigned int x, unsigned int y, uint32_t color) {
    set_pixel(x, y, sf::Color(color));
}

void Screen::set_pixel(unsigned int x, unsigned int y, const sf::Color &color) {
    if (x >= image_.getSize().x) {
        return;
    }
    if (y >= image_.getSize().y) {
        return;
    }

    image_.setPixel(x, y, color);
}

void Screen::draw(sf::RenderWindow &window) {
    texture_.loadFromImage(image_);
    sf::Sprite sprite;
    sprite.setTexture(texture_, true);
    sprite.setScale(scaling_, scaling_);

    window.draw(sprite);
}

} // namespace nesvis
