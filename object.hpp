#pragma once
#include "headers.hpp"

class Object
{

public:
    sf::Texture tex;
    sf::Sprite sprite;

    float size_multiplier;

    v2f position;
    v2f rotation;
    float direction;

    Object(int x, int y);
    void setScale(float scale);

};

