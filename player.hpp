#pragma once

#include "headers.hpp"
#include "map.hpp"
#include "object.hpp"

class Player
{
private:
	// game logic
	sf::RenderWindow& window;
	Map& map;
	

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
	float max_hand_range = 60;
	float hand_move_range;

	// movement
	v2f position;
	float speed = 3.0f;
	bool running = false, moving = false;
	float run_multiplier = 1.75f;

	// orientation
	float rotation_x = -3.169f;
	float rotation_y = -0.56f;
	float mouse_sensitivity = 0.8f;
	float fov_y = 0.7f;
	float fov_x = 1.22173f; // 70 degrees

	// map
	float body_radius = 0.4f;

public:
	
	struct HitInfo {
		float distance;
		bool on_x_axis;
		float texture_x;

		float perceived_distance;
	};

	Player(int x, int y, Map& map, sf::RenderWindow& window);

	void handleKeys(float dt);
	void rotateHead(int delta_x, int delta_y, float dt);
	void move(float angle_offset, float dt);

	void shootRays(HitInfo*& hits);
	void drawWorld(HitInfo*& hits, vector<Object>& objects);
	void drawColumn(int x, const Player::HitInfo& hit_info);
	Player::HitInfo shootRay(float angle_offset);
	
	void drawObject(Object& object);
	void drawGun(float dt);
	void drawCrosshair();
	void shootGun();

	void loadTextures();
	

	//
	void debug();

};
