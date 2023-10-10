#include "headers.hpp"
#include "map.hpp"


Map::Map(int x, int y, int w, int h, sf::RenderWindow& window) : position(x, y), window(window)
{
	data = new int[100]
		{
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 1, 1, 1, 0, 0, 1,
			1, 0, 0, 0, 0, 1, 1, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1
		};

	width = w;
	height = h;

}

int Map::getCell(int x, int y)
{
	return data[x + width * y];
}

void Map::drawMap()
{
	sf::RectangleShape cell(v2f(cellSize, cellSize));

	cell.setOutlineColor(sf::Color(70, 70, 80));
	cell.setOutlineThickness(-1);

	sf::Color colors[]{
		sf::Color::Black,
		sf::Color::White
	};

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int color_index = getCell(i, j);
			cell.setFillColor(colors[color_index]);
			cell.setPosition(position.x + cellSize * i, position.y + cellSize * j);
			window.draw(cell);
		}
	}
}

void Map::drawPoint(float x, float y)
{
	sf::CircleShape dot(5);

	dot.setFillColor(sf::Color::Red);

	dot.setPosition(position.x + cellSize * x - 2.5f, position.y + cellSize * y - 2.5f);
	
	window.draw(dot);
		
}