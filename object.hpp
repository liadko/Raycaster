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

    vector<vector<sf::IntRect>> directions_animations =
    {
        { { 1, 1, 81, 109 }, { 1, 133, 73, 109 }, { 1, 265, 75, 109 }, { 1, 397, 79, 109 }, },
        { { 87, 1, 71, 107 }, { 80, 133, 68, 107 }, { 81, 265, 73, 107 }, { 86, 397, 68, 107 }, },
        { { 163, 1, 85, 105 }, { 153, 133, 70, 109 }, { 159, 265, 73, 109 }, { 159, 397, 67, 107 }, },
        { { 253, 1, 89, 99 }, { 230, 133, 78, 107 }, { 238, 265, 80, 109 }, { 232, 397, 80, 107 }, },
        { { 347, 1, 71, 101 }, { 313, 133, 69, 107 }, { 324, 265, 66, 109 }, { 317, 397, 69, 107 }, },
        { { 433, 23, 65, 101 }, { 399, 147, 63, 109 }, { 399, 283, 73, 107 }, { 400, 415, 68, 107 }, },
        { { 511, 21, 89, 103 }, { 485, 147, 74, 109 }, { 492, 279, 72, 111 }, { 493, 411, 71, 111 }, },
        { { 615, 21, 83, 105 }, { 586, 147, 74, 107 }, { 592, 279, 78, 111 }, { 591, 411, 77, 109 }, },
    };

    int animation_index = 0;
    float animation_timer = 0;

    Object(float x, float y, const sf::Texture& tex, float scaler);
    float distFrom(const v2f& pos);
    void animate(float dt);
};

