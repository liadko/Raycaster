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

void Object::loadPlayerInfo(Client::PlayerInfo player_info)
{
    position.x = player_info.pos_x;
    position.y = player_info.pos_y;


    rotation_x = player_info.rot_x;

    bool moving_flag = player_info.flags & Client::PlayerInfo::Flag::moving;
    bool forward_flag = player_info.flags & Client::PlayerInfo::Flag::forward;
    bool shot_gun_flag = player_info.flags & Client::PlayerInfo::Flag::gun_shot;


    if (moving_flag != moving)
    {
        //player started or stopped moving
        started_moving = true;
    }

    moving = moving_flag;
    forward = forward_flag;
}

sf::IntRect Object::getTextureRect(float rotation, float frame)
{
    return sf::IntRect(120 * rotation, 120 * frame, 120, 120);
}