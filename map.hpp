#pragma once

class Map
{
private:
	int* data;
	sf::RenderWindow& window;

	sf::Texture sky_tex;
	sf::Sprite sky_sprite;

public:
	int width, height;
	v2i position;

	int cell_size = 4;

	float floor_level = 0;

	float sky_offset = 0;
	float sky_scale = 2.5f;
	float sky_width = 1833;
	float sky_sensitivity = -1000;

	sf::Color sky_color, ground_color;

	Map(int dist_from_side, sf::RenderWindow& window);
	int getCell(int x, int y);
	void drawMap();
	void drawPoint(float x, float y);

	void drawGround();
	void drawSky();
	void shiftSky(float offset);
};