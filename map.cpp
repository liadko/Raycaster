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

	cellSize = 60;
}

int Map::getCell(int x, int y)
{
	return data[x + width * y];
}

void Map::drawMap()
{
	sf::RectangleShape cell(v2(cellSize, cellSize));

	cell.setOutlineColor(sf::Color(70, 70, 80));
	cell.setOutlineThickness(1);

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