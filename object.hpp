#pragma once
#include "headers.hpp"

class Object
{

public:
    sf::Sprite sprite;

    v2f tex_size;
    float size_multiplier;

    v2f position;
    v2f rotation;
    float direction;

    Object(float x, float y, const sf::Texture& tex, float scaler);
    float distFrom(const v2f& pos);
};

