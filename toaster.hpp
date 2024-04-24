#pragma once
#include "headers.hpp"
#include "tools.hpp"

class Toaster
{
private:
    vector<string> toasts;
    vector<float> toast_slides;
    vector<float> toast_timers;

    v2f first_toast_position;
    float goal_y = 0;
    v2f toast_size = { 250, 80 };

    sf::Texture toast_tex;
    sf::Sprite toast_sprite;

    sf::Font font;
    sf::Text text;

    v2f text_position = { 36, 40 };

    float lifetime = 6; // in seconds

public:
    Toaster();
    void drawToasts(sf::RenderWindow& window, float dt);
    void toast(const string& text);
};