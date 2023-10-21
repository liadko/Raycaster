#pragma once

#include "headers.hpp"
#include "map.hpp"

class Player
{
private:
	sf::RenderWindow& window;
	Map& map;
	

	sf::Texture* wall_texs;
	
	int gun_animation_frame;
	float gun_animation_timer;
	sf::Sprite gun_sprite;
	sf::Texture* gun_texs;
	

	struct HitInfo {
		float distance;
		bool on_x_axis;
		float texture_x;
	};

public:
	

	float body_radius = 0.4f;
	v2f position;
	
	float speed = 3.0f;
	bool running = false;
	float run_multiplier = 1.75f;

	float rotation_x = -0.1f;
	float rotation_y = -0.52f;

	float mouse_sensitivity = 1.1f;
	float fov_y = 0.7f;


	float fov_x = 1.22173f;
	float render_distance = 20.0f;

	Player(int x, int y, Map& map, sf::RenderWindow& window);


	void handleKeys(float dt);
	void rotateHead(int delta_x, int delta_y, float dt);
	void move(float angle_offset, float dt);

	void shootRays();
	Player::HitInfo shootRay(float angle_offset);
	
	void drawGun(float dt);
	void drawCrosshair();
	void shootGun();

	void loadTextures();

};
