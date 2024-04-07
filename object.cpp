#include "headers.hpp"
#include "object.hpp"



Object::Object(float x, float y, const sf::Texture& tex, float scaler)
    : position(x, y), direction_index(0), scale_by(scaler)
{
    tex_size = (v2f)tex.getSize();
    sprite.setTexture(tex, true);
    sprite.setTextureRect(directions_animations[direction_index][0]);
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

        animation_index = (animation_index + 1) % directions_animations[1].size();
        sprite.setTextureRect(directions_animations[direction_index][animation_index]);
    }
}