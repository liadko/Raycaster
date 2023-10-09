#include "headers.hpp"
#include "player.hpp"
#include "graphics.hpp"

Player::Player(int x, int y, Map& map, sf::RenderWindow& window) : position(x, y), map(map), window(window)
{

	if (!texture.loadFromFile("turret.png")) {
		// Handle loading error if the image file is not found
		cout << "Can't Find File.\n";
		return;
	}

	sprite.setTexture(texture);
	//sprite.setScale(0.07, 0.07); // scale 0.07 works for cellSize=24
	sprite.setScale(0.175, 0.175);
	sprite.setPosition(v2(map.position) + position * (float)map.cellSize);

	// set anchor point
	sf::Vector2f center(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
	sprite.setOrigin(center);

	rotation = 0;

}

void Player::draw()
{
	window.draw(sprite);
}

void Player::rotate(float alpha, float dt)
{
	rotation += alpha * rotation_speed * dt;
	sprite.setRotation(rotation / PI * 180);
}


void Player::move(float rotation_offset, float dt)
{
	v2 potential_position;
	potential_position.x = position.x + speed * dt * cos(rotation - rotation_offset);
	potential_position.y = position.y + speed * dt * sin(rotation - rotation_offset);

	v2i ones(1, 1);
	v2i predicted_cell = v2i(floor(potential_position.x), floor(potential_position.y));

	v2i area_tl = min((v2i)position, predicted_cell);
	area_tl = max({ 0, 0 }, area_tl - ones); // clamp


	v2i area_br = max((v2i)position, predicted_cell);
	area_br = min({ map.width, map.height }, area_br + ones); // clamp


	for (int cell_x = area_tl.x; cell_x <= area_br.x; cell_x++)
	{
		for (int cell_y = area_tl.y; cell_y <= area_br.y; cell_y++)
		{

			// is wall
			if (map.getCell(cell_x, cell_y) == 1)
			{
				v2 nearest_point;
				nearest_point.x = std::max(float(cell_x), std::min(potential_position.x, float(cell_x + 1)));
				nearest_point.y = std::max(float(cell_y), std::min(potential_position.y, float(cell_y + 1)));

				v2 ray_to_nearest = nearest_point - potential_position;

				float overlap = radius - mag(ray_to_nearest);

				if (std::isnan(overlap)) overlap = 0;

				if (overlap > 0)
				{
					potential_position -= norm(ray_to_nearest) * overlap;
				}
			}

		}
	}

	position = potential_position;
	sprite.setPosition(v2(map.position) + position * (float)map.cellSize);
}