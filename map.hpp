#pragma once

class Map
{
private:
	int* data;
	sf::RenderWindow& window;

public:
	int width, height;
	v2i position;

	int cellSize = 24;

	Map(int x, int y, int w, int h, sf::RenderWindow& window);
	int getCell(int x, int y);
	void drawMap();
	void drawPoint(float x, float y);
};