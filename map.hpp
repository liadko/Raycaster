#pragma once

class Map
{
private:
	int* data;
	sf::RenderWindow& window;

public:
	int width, height;
	v2i position;

	int cell_size = 4;

	sf::Color sky_color, ground_color;

	Map(int dist_from_side, sf::RenderWindow& window);
	int getCell(int x, int y);
	void drawMap();
	void drawGround(float floor_level);
	void drawPoint(float x, float y);
};