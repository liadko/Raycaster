#include "headers.hpp"
#include "player.hpp"
#include "graphics.hpp"

#include <cstdlib>
#include <time.h>

Player::Player(int x, int y, Map& map, sf::RenderWindow& window) : position(x, y), map(map), window(window)
{

	if (!texture.loadFromFile("turret.png")) {
		// Handle loading error if the image file is not found
		cout << "Can't Find File.\n";
		return;
	}

	sprite.setTexture(texture);
	sprite.setScale(0.025, 0.025); // scale 0.07 works for cellSize=24
	//sprite.setScale(0.175, 0.175); // scale 0.175 works for cellSize=60
	sprite.setPosition(v2f(map.position) + position * (float)map.cell_size);

	// set anchor point
	sf::Vector2f center(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
	sprite.setOrigin(center);
	
	srand(time(0));
	rotation = 0.1;

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

void Player::move(float angle_offset, float dt)
{
	float current_speed = speed;
	if (running)
		current_speed *= run_multiplier;

	v2f potential_position;
	potential_position.x = position.x + current_speed * run_multiplier * dt * cos(rotation - angle_offset);
	potential_position.y = position.y + current_speed * run_multiplier * dt * sin(rotation - angle_offset);

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
				v2f nearest_point;
				nearest_point.x = std::max(float(cell_x), std::min(potential_position.x, float(cell_x + 1)));
				nearest_point.y = std::max(float(cell_y), std::min(potential_position.y, float(cell_y + 1)));

				v2f ray_to_nearest = nearest_point - potential_position;

				float overlap = body_radius - mag(ray_to_nearest);

				if (std::isnan(overlap)) overlap = 0;

				if (overlap > 0)
				{
					potential_position -= norm(ray_to_nearest) * overlap;
				}
			}

		}
	}

	position = potential_position;
	sprite.setPosition(v2f(map.position) + position * (float)map.cell_size);
}

// returns distance in that direction
Player::HitInfo Player::shootRay(float angle_offset)
{
	v2f ray_dir(cos(rotation + angle_offset), sin(rotation + angle_offset));

	v2f ray_unit_step_size;
	ray_unit_step_size.x = sqrt(1 + (ray_dir.y/ ray_dir.x) * (ray_dir.y / ray_dir.x));
	ray_unit_step_size.y = sqrt(1 + (ray_dir.x/ ray_dir.y) * (ray_dir.x / ray_dir.y));


	v2i current_cell(position);

	v2f ray_length;
	v2i cell_step;

	if (ray_dir.x < 0)
	{
		cell_step.x = -1;
		ray_length.x = (position.x - (float)current_cell.x) * ray_unit_step_size.x;
	}
	else
	{
		cell_step.x = 1;
		ray_length.x = (float(current_cell.x + 1) - position.x) * ray_unit_step_size.x;
	}

	if (ray_dir.y < 0)
	{
		cell_step.y = -1;
		ray_length.y = (position.y - (float)current_cell.y) * ray_unit_step_size.y;
	}
	else
	{
		cell_step.y = 1;
		ray_length.y = (float(current_cell.y + 1) - position.y) * ray_unit_step_size.y;
	}

	float distance = 0;
	bool tile_found = false;
	bool latest_hit_on_x;
	while (!tile_found)
	{
		if (ray_length.x < ray_length.y)
		{
			current_cell.x += cell_step.x;
			distance = ray_length.x;
			latest_hit_on_x = true;
			ray_length.x += ray_unit_step_size.x;
		}
		else
		{
			current_cell.y += cell_step.y;
			distance = ray_length.y;
			latest_hit_on_x = false;
			ray_length.y += ray_unit_step_size.y;

		}

		if (current_cell.x < 0 || current_cell.y < 0 || current_cell.x >= map.width || current_cell.y >= map.height)
			return { -1 };

		//hit wall
		if (map.getCell(current_cell.x, current_cell.y) == 1)
		{
			return { distance, latest_hit_on_x };
		}

	}


}


void Player::shootRays()
{
	sf::RectangleShape rect(v2f(1, HEIGHT));
	rect.setFillColor(sf::Color::White);

	float angle_offset = -FOV / 2;
	float step = FOV / WIDTH;


	// For each column of pixels
	for (int x = 0; x < WIDTH; x++)
	{
		HitInfo hit_info = shootRay(angle_offset);

		float dist = hit_info.distance * cos(angle_offset);
		float len = 1000 / dist;

		if(!hit_info.on_x_axis)
			rect.setFillColor(sf::Color(140, 140, 150));
		else
			rect.setFillColor(sf::Color(240, 240, 245));

		rect.setPosition(x, 360 - len / 2);
		rect.setSize(v2f(1, len));
		window.draw(rect);

		angle_offset += step;
	}
}

void Player::drawCrosshair()
{
	sf::RectangleShape rect(v2f(4, 12));
	rect.setFillColor(sf::Color::Cyan);
	
	rect.setPosition(v2f(WIDTH /2 - 2, HEIGHT /2 - 16));
	window.draw(rect);

	rect.setPosition(v2f(WIDTH / 2 - 2, HEIGHT / 2 + 4));
	window.draw(rect);


	rect.setSize(v2f(12, 4));
	rect.setPosition(v2f(WIDTH / 2 - 16, HEIGHT / 2 - 2));
	window.draw(rect);

	rect.setPosition(v2f(WIDTH / 2 + 4, HEIGHT / 2 - 2));
	window.draw(rect);

}