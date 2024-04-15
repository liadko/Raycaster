#include "headers.hpp"
#include "object.hpp"
#include "client.hpp" // for struct PlayerInfo


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
    if (!moving)
    {
        sprite.setTextureRect(getTextureRect(direction_index, 4)); // 4 - standing frame
        return;
    }
    float interval = 0.2f;
    animation_timer += dt;
    if (started_moving || animation_timer > interval)
    {
        animation_timer -= interval;

        // there are 4 animation frames
        if (forward)
            animation_index = (animation_index + 1) % 4;
        else
            animation_index = (animation_index - 1 + 4) % 4;

        sprite.setTextureRect(getTextureRect(direction_index, animation_index));

        started_moving = false;
    }
}

void Object::loadPlayerInfo(char* player_info_buffer)
{
    Client::PlayerInfo player_info = *(Client::PlayerInfo*)(player_info_buffer);

    position.x = player_info.pos_x;
    position.y = player_info.pos_y;

    rotation_x = player_info.rot_x;

    if (player_info.moving != moving)
    {
        //player started or stopped moving
        started_moving = true;
    }

    moving = player_info.moving;
    forward = player_info.forward;
}

sf::IntRect Object::getTextureRect(float rotation, float frame)
{
    return sf::IntRect(120 * rotation, 120 * frame, 120, 120);
}