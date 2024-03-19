#include "headers.hpp"
#include "object.hpp"



Object::Object(int x, int y)
    : position(x, y), direction(0), size_multiplier(0.0018f)
{
    tex.loadFromFile("sprites/cop.png");

    sprite.setTexture(tex);
    sprite.setPosition(WIDTH / 2, HEIGHT / 2);
}

void Object::setScale(float scale)
{

}