#include "headers.hpp"
#include "object.hpp"



Object::Object(float x, float y, const sf::Texture& tex, float scaler)
    : position(x, y), direction(0), size_multiplier(scaler)
{
    tex_size = (v2f)tex.getSize();
    sprite.setTexture(tex, true);
    sprite.setTextureRect(sf::IntRect(1, 529, 51, 109));
    shrink_multiplier = 0.5f;
}

float Object::distFrom(const v2f& pos)
{
    return sqrtf(pow(pos.x - position.x, 2) + pow(pos.y - position.y, 2));
}