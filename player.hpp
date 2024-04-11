#pragma once

#include "headers.hpp"
#include "map.hpp"
#include "object.hpp"
#include "client.hpp"
#include <SFML/Audio.hpp>

class Player
{
private:
	// game logic
	sf::RenderWindow& window;
	

	sf::Texture* wall_texs;
	sf::Sprite wall_sprite;

	// gun animation
	int gun_animation_frame;
	float gun_animation_timer, gun_movement_stopwatch;
	float* gun_animation_duration;
	sf::Sprite gun_sprite;
	sf::Texture* gun_texs;
	v2f gun_position;
	v2f gun_offset;
	float max_hand_range = 40;
	float hand_move_range;

	// movement
	v2f position;
	float speed = 2.0f;
	bool running = false, crouching = false;
	bool moving = false;
	float run_multiplier = 1.75f, crouch_multiplier = 0.5f;

	// orientation
	float rotation_x = -3.169f;
	float rotation_y = -0.56f;
	float mouse_sensitivity = 0.8f;
	float fov_y = 0.7f;
	float fov_x = 1.22173f; // 70 degrees

	// map
	float body_radius = 0.4f;

	//sound
	sf::SoundBuffer gunshot_buffer, gunclick_buffer;
	sf::Sound gun_sound, click_sound;

public:
	Client client;
	Map map;
	bool window_focused;

	struct HitInfo {
		float distance;
		bool on_x_axis;
		float texture_x;

		float perceived_distance;
	};

	struct PlayerInfo
	{
		float x, y, rot_x, rot_y;
	};

	Player(int x, int y, sf::RenderWindow& window);

	void setFocus(bool focus);
	void handleKeys(float dt);
	void rotateHead(int delta_x, int delta_y, float dt);
	void move(float angle_offset, float dt);

	void shootRays(HitInfo*& hits);
	void drawWorld(HitInfo*& hits, vector<Object>& objects, float dt);
	void drawColumn(int x, const Player::HitInfo& hit_info);
	Player::HitInfo shootRay(float angle_offset);
	
	void drawObject(Object& object, float dt);
	void drawGun(float dt);
	void drawCrosshair();
	void shootGun(bool left_click);

	void loadTextures();
	

	//server
	void updateServer();

	//
	void debug();

};
