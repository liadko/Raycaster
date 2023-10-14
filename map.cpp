#include "headers.hpp"
#include "map.hpp"


Map::Map(int dist_from_side, sf::RenderWindow& window) : window(window)
{
	sf::Image map_texture;
	map_texture.loadFromFile("map.png");

	width = map_texture.getSize().x;
	height = map_texture.getSize().y;

	

	data = new int[width * height];
	
	for(int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			//cout << map_texture.getPixel(x, y).r << '\n';
			if (map_texture.getPixel(i, j).r == 255)
				data[i + j * width] = 1;
			else
				data[i + j * width] = 0;

		}


	map_texture.~Image();

	position.x = dist_from_side;
	position.y = HEIGHT - dist_from_side - cell_size * height;

	sky_color = sf::Color(70, 170, 255);
	ground_color = sf::Color(7, 130, 33);
}

int Map::getCell(int x, int y)
{
	return data[x + width * y];
}

void Map::drawMap()
{
	sf::RectangleShape cell(v2f(cell_size, cell_size));

	// cell.setOutlineColor(sf::Color(70, 70, 80));
	// cell.setOutlineThickness(1);

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
			cell.setPosition(position.x + cell_size * i, position.y + cell_size * j);
			window.draw(cell);
		}
	}
}

void Map::drawPoint(float x, float y)
{
	sf::CircleShape dot(5);

	dot.setFillColor(sf::Color::Red);

	dot.setPosition(position.x + cell_size * x - 2.5f, position.y + cell_size * y - 2.5f);
	
	window.draw(dot);
		
}

void Map::drawGround(float ground_level)
{
	//cout << width << " "<< height << "\n";
	float ground_height = HEIGHT - ground_level;

	sf::RectangleShape ground(v2f(WIDTH, ground_height));

	ground.setFillColor(ground_color);
	ground.setPosition(v2f(0, ground_level));
	window.draw(ground);
}