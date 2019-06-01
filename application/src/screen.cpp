#include "screen.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

Screen::Screen(unsigned int width, unsigned int height, unsigned int pixel_size)
        : pixel_size_(pixel_size), image_() {
    image_.create(
            width * pixel_size_, height * pixel_size_, sf::Color(0, 0, 0));
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
    unsigned int scaled_x = x * pixel_size_;
    unsigned int scaled_y = y * pixel_size_;

    if (scaled_x >= image_.getSize().x) {
        return;
    }
    if (scaled_y >= image_.getSize().y) {
        return;
    }

    for (unsigned int ix = 0; ix < pixel_size_; ++ix) {
        for (unsigned int iy = 0; iy < pixel_size_; ++iy) {
            image_.setPixel(scaled_x + ix, scaled_y + iy, color);
        }
    }
}

void Screen::draw(sf::RenderWindow &window) {
    sf::Texture texture;
    texture.loadFromImage(image_);
    sf::Sprite sprite;
    sprite.setTexture(texture, true);

    window.draw(sprite);
}
