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

	vector<sf::Texture> textures;
	
	float cos_a, sin_a, tan_a; // cached trig values of rotation

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

	float mouse_sensetivity = 0.6f;
	float fov_y = 0.7f;


	float fov_x = 1.22173f;
	float render_distance = 20.0f;

	Player(int x, int y, Map& map, sf::RenderWindow& window);

	void handleKeys(float dt);
	void rotateHead(int delta_x, int delta_y, float dt);
	void move(float angle_offset, float dt);

	void shootRays();
	Player::HitInfo shootRay(float angle_offset);
	
	void draw();
	void drawCrosshair();

};
