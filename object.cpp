#include "headers.hpp"
#include "object.hpp"



Object::Object(float x, float y, const sf::Texture& tex, float scaler)
    : position(x, y), direction_index(0), scale_by(scaler)
{
    tex_size = (v2f)tex.getSize();
    sprite.setTexture(tex, true);
    sprite.setTextureRect(getTextureRect(direction_index, 0));
    shrink_by = 0.75f;

    
}

float Object::distFrom(const v2f& pos)
{
    return sqrtf(pow(pos.x - position.x, 2) + pow(pos.y - position.y, 2));
}


void Object::animate(float dt)
{
    float interval = 0.2f;
    animation_timer += dt;
    if (animation_timer > interval)
    {
        animation_timer -= interval;

        animation_index = (animation_index + 1) % 4; // there are 4 animation frames
        sprite.setTextureRect(getTextureRect(direction_index, animation_index));
    }
}

sf::IntRect Object::getTextureRect(float rotation, float frame)
{
    return sf::IntRect(120 * rotation, 120 * frame, 120, 120);
}