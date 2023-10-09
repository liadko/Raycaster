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
	
	float radius = 0.4;
	v2 position;
	float rotation;
	float rotation_speed = 5;
	float speed = 4;

	Player(int x, int y, Map& map, sf::RenderWindow& window);

	void draw();
	void rotate(float alpha, float dt);
	void move(float rotation_offset, float dt);
};
