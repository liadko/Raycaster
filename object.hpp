#pragma once
#include "headers.hpp"

class Object
{

public:
    sf::Sprite sprite;

    v2f tex_size;
    float scale_by, shrink_by;

    v2f position;
    float rotation_x;
    int direction_index; // 0, 1, 2, 3, 4, 5, 6, 7


    int animation_index = 0;
    float animation_timer = 0;

    Object(float x, float y, const sf::Texture& tex, float scaler);
    float distFrom(const v2f& pos);
    void animate(float dt);
    void loadPlayerInfo(char* player_info_buffer);


    static sf::IntRect getTextureRect(float rotation, float frame);
};

