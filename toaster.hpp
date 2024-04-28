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

    // leaderboard

    sf::Texture notch_tex, board_tex;
    sf::Sprite notch_sprite, board_sprite;


    v2f leaderboard_position = { 20, 10 };
    float leaderboard_scale = 0.8f;

    sf::Text leaderboard_text;
    v2f leaderboard_text_offset = { 9, 6 };

public:
    struct LeaderboardEntry
    {
        int player_id;
        string username;
        int score;

        float position_y;

        LeaderboardEntry(int player_id, const string& username);
    };

    Toaster();
    void drawToasts(sf::RenderWindow& window, float dt);
    void drawLeaderboard(sf::RenderWindow& window,  vector<LeaderboardEntry>& leaderboard, float dt);
    void toast(const string& text);
};