#pragma once
#include "headers.hpp"
#include "client.hpp"

class Object
{

public:
    sf::Sprite sprite;

    v2f tex_size;
    float scale_by, shrink_by;

    v2f position;
    float rotation_x;
    int direction_index; // 0, 1, 2, 3, 4, 5, 6, 7

    bool moving = false, started_moving = false, forward;
    int animation_index = 0;
    float animation_timer = 0;

    bool shooting_gun = false;
    float gun_timer = 0;

    bool getting_shot;
    float getting_shot_timer = 0;

    int player_id = -1;

    Object();
    Object(float x, float y, const sf::Texture& tex);
    float distFrom(const v2f& pos);

    void loadPlayerInfo(Client::PlayerInfo player_info);

    void animate(float dt);
    void shootGun();
    void gotShot();

    static sf::IntRect getTextureRect(float rotation, float frame);
};

