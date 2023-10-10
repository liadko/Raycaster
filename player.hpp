#pragma once

#include "headers.hpp"
#include "map.hpp"

class Player
{
private:
	sf::Texture texture;
	sf::Sprite sprite;
	Map& map;
	sf::RenderWindow& window;

	

public:
	
	struct HitInfo {
		float distance;
		bool on_x_axis;
	};

	float body_radius = 0.4;
	v2f position;
	
	float rotation;
	float rotation_speed = 3;
	float speed = 4;

	bool running = false;
	float run_multiplier = 1.5;

	float FOV = 1.22173;

	Player(int x, int y, Map& map, sf::RenderWindow& window);

	void draw();
	void rotate(float alpha, float dt);
	void move(float angle_offset, float dt);
	void shootRays();
	Player::HitInfo shootRay(float angle_offset);
	void drawCrosshair();

};
