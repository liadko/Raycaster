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
    if (shooting_gun)
    {
        gun_timer += dt;

        if (gun_timer > 0.4f)
            shooting_gun = false;

        if (gun_timer > 0.3f)
            sprite.setTextureRect(getTextureRect(direction_index, 5));
        else if (gun_timer > 0.1f)
            sprite.setTextureRect(getTextureRect(direction_index, 5));
        else
            sprite.setTextureRect(getTextureRect(direction_index, 5));

        return;
    }

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

void Object::shootGun()
{
    gun_timer = 0;
    shooting_gun = true;
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


    if (shot_gun_flag)
        shootGun();

}

sf::IntRect Object::getTextureRect(float rotation, float frame)
{
    return sf::IntRect(280 * rotation, 280 * frame, 280, 280);
}

Object::Object()
{

}