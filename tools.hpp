#pragma once
#include "headers.hpp"
#include <sstream>

v2i min(const v2i& first, const v2i& second);

v2i max(const v2i& first, const v2i& second);

float mag(const v2f& vec);

v2f norm(const v2f& vec);

float lerp(float a, float b, float t);

sf::Color lerp(sf::Color c1, sf::Color c2, float t);

bool inBounds(const v2f& box_pos, const v2f& box_size, const v2i& pos);

vector<string> split(const string& str);

float angleBetweenVectors(const v2f& v1, const v2f& v2);


struct TextBox
{
    v2f position, size;

    string text_string;
    sf::Text text;
    v2f text_offset = { 20, 16 };

    sf::RectangleShape shadow;

    sf::Clock cursor_timer;

    sf::RectangleShape cursor;
    bool cursor_visible = true;

    bool hidden = false;

    TextBox(const v2f& pos, const v2f& size, const string& str, const sf::Font& font);


    void addText(const string& added_text);
    void backspace(const int& backspace_counter);
    void draw(sf::RenderWindow& window, bool is_focused);
    
    string getString();
    bool inBox(const v2i& pos);

    void turnOnCursor();
};