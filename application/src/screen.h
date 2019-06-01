#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>

#include <cstdint>

class Screen {
public:
    Screen(unsigned int width, unsigned int height, unsigned int pixel_size);

    void draw(sf::RenderWindow& window);

    void set_pixel(unsigned int x,
            unsigned int y,
            uint8_t r,
            uint8_t g,
            uint8_t b);
    void set_pixel(unsigned int x, unsigned int y, uint32_t color);
    void set_pixel(unsigned int x, unsigned int y, const sf::Color& color);

private:
    unsigned int pixel_size_;

    sf::Image image_;
};
