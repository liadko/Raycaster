#pragma once
#include "headers.hpp"

v2i min(const v2i& first, const v2i& second);

v2i max(const v2i& first, const v2i& second);

float mag(const v2f& vec);

v2f norm(const v2f& vec);

float lerp(float a, float b, float t);

sf::Color lerp(sf::Color c1, sf::Color c2, float t);

bool inBounds(const v2f& box_pos, const v2f& box_size, const v2i& pos);

struct TextBox
{
    v2f position, size;

    sf::Text text;
    v2f text_offset = { 20, 16 };

    bool is_focused;
    sf::RectangleShape shadow;

    sf::Clock cursor_timer;

    sf::RectangleShape cursor;
    bool cursor_visible = true;

    TextBox(const v2f& pos, const v2f& size, const string& str, const sf::Font& font);


    void addText(const string& added_text);
    void draw(sf::RenderWindow& window);
    
    string getString();
    bool inBounds(const v2i& pos);
};