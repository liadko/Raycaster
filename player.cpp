#include "headers.hpp"
#include "player.hpp"
#include "tools.hpp"

Player::Player(int x, int y, Map& map, sf::RenderWindow& window) : position(x, y), map(map), window(window)
{
	loadTextures();

	// set anchor point
	//sf::Vector2f center(sprite.getLocalBounds().width / 2.0f, sprite.getLocalBounds().height / 2.0f);
	//sprite.setOrigin(center);


}



#define KEY_PRESSED(key) sf::Keyboard::isKeyPressed(sf::Keyboard::Key::key)
void Player::handleKeys(float dt)
{
	// move
	v2f movement(0, 0);
	if (KEY_PRESSED(W))
		movement.y += +1;
	if (KEY_PRESSED(A))
		movement.x += 1;
	if (KEY_PRESSED(S))
		movement.y += -1;
	if (KEY_PRESSED(D))
		movement.x += -1;

	if (!(movement.x == 0 && movement.y == 0))
		move(atan2(movement.x, movement.y), dt);

	// run
	if (KEY_PRESSED(LShift))
		running = true;
	else
		running = false;


	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	//	map.sky_offset += 1833*map.sky_scale;


	if (KEY_PRESSED(Escape))
		window.close();
}

void Player::rotateHead(int delta_x, int delta_y, float dt)
{
	// ignore enourmous rotation requests
	if (mag(v2f(delta_x, delta_y)) > 120)
	{
		cout << mag(v2f(delta_x, delta_y)) << "\n";
		return;
	}


	// horizontal
	rotation_x += delta_x * mouse_sensitivity * dt;
	//sprite.setRotation(rotation_x / PI * 180);		// update player map sprite
	map.shiftSky(delta_x * mouse_sensitivity * dt); // shift sky


	// vertical
	rotation_y += delta_y * -mouse_sensitivity * dt;

	map.floor_level = HEIGHT / 2 * (1 + tan(rotation_y)) / tan(fov_y / 2);

	// cap y rotation
	if (rotation_y > 0.75f)
		rotation_y = 0.75f;

	if (rotation_y < -1.05f)
		rotation_y = -1.05f;

}

void Player::move(float angle_offset, float dt)
{
	float current_speed = speed;
	if (running)
		current_speed *= run_multiplier;

	v2f potential_position;
	potential_position.x = position.x + current_speed * run_multiplier * dt * cos(rotation_x - angle_offset);
	potential_position.y = position.y + current_speed * run_multiplier * dt * sin(rotation_x - angle_offset);

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
	//gun_sprite.setPosition(v2f(map.position) + position * (float)map.cell_size);
}

// returns distance in that direction
Player::HitInfo Player::shootRay(float angle_offset)
{
	//angle_offset = angle_offset * (tan(angle_offset));

	v2f ray_dir(cos(rotation_x + angle_offset), sin(rotation_x + angle_offset));

	v2f ray_unit_step_size;
	ray_unit_step_size.x = sqrt(1 + (ray_dir.y / ray_dir.x) * (ray_dir.y / ray_dir.x));
	ray_unit_step_size.y = sqrt(1 + (ray_dir.x / ray_dir.y) * (ray_dir.x / ray_dir.y));

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
			latest_hit_on_x = false;
			ray_length.x += ray_unit_step_size.x;
		}
		else
		{
			current_cell.y += cell_step.y;
			distance = ray_length.y;
			latest_hit_on_x = true;
			ray_length.y += ray_unit_step_size.y;

		}

		if (current_cell.x < 0 || current_cell.y < 0 || current_cell.x >= map.width || current_cell.y >= map.height)
			return { -1 };

		//hit wall
		if (map.getCell(current_cell.x, current_cell.y) == 1)
		{
			v2f hit_position = position + ray_dir * distance;
			//getting the x offset on the texture
			float texture_x;

			if (latest_hit_on_x)
			{
				texture_x = hit_position.x - floor(hit_position.x);
				if (sin(rotation_x) > 0) texture_x = 1 - texture_x;
			}
			else
			{
				texture_x = hit_position.y - floor(hit_position.y);
				if (cos(rotation_x) < 0) texture_x = 1 - texture_x;
			}


			return { distance, latest_hit_on_x, texture_x };
		}

	}


}

void Player::shootRays()
{
	sf::Sprite sprite;
	sprite.setTexture(wall_texs[0]);


	float angle_offset = -fov_x / 2;
	float step = fov_x / WIDTH;


	map.drawGround();

	// For each column of pixels
	for (int x = 0; x < WIDTH; x++)
	{
		HitInfo hit_info = shootRay(angle_offset);

		float dist = hit_info.distance * cos(angle_offset);

		float len = 1000 / dist;

		//sf::Color color(240, 240, 245);
		if (hit_info.on_x_axis)
			sprite.setTexture(wall_texs[1]);
		else
			sprite.setTexture(wall_texs[0]);

		//color = sf::Color(hit_info.texture_x * 255, 0, 0);


		sprite.setTextureRect(sf::IntRect(v2i(hit_info.texture_x * wall_texs[0].getSize().x, 0), v2i(1, wall_texs[0].getSize().y)));
		sprite.setScale(1, len / wall_texs[0].getSize().y);
		sprite.setPosition(x, map.floor_level - len / 2);
		window.draw(sprite);



		angle_offset += step;
	}
}


void Player::drawCrosshair()
{
	sf::RectangleShape rect(v2f(4, 12));
	rect.setFillColor(sf::Color::Cyan);

	rect.setPosition(v2f(WIDTH / 2 - 2, HEIGHT / 2 - 16));
	window.draw(rect);

	rect.setPosition(v2f(WIDTH / 2 - 2, HEIGHT / 2 + 4));
	window.draw(rect);


	rect.setSize(v2f(12, 4));
	rect.setPosition(v2f(WIDTH / 2 - 16, HEIGHT / 2 - 2));
	window.draw(rect);

	rect.setPosition(v2f(WIDTH / 2 + 4, HEIGHT / 2 - 2));
	window.draw(rect);

}


void Player::loadTextures()
{
	// walls
	wall_texs = new sf::Texture[2]; // (sf::Texture*)malloc(sizeof(sf::Texture) * 2);

	sf::Texture wall;
	wall.loadFromFile("1L.png");
	wall_texs[0] = wall;
	wall.loadFromFile("1D.png");
	wall_texs[1] = wall;


	// gun
	gun_texs = new sf::Texture[6]();


	for (int i = 0; i < 6; i++)
	{
		char filename[] = "gun_animation/gun_X.png";
		filename[18] = i + '0';
		gun_texs[i].loadFromFile(filename);
	}

	gun_sprite.setTexture(gun_texs[0]);
	gun_sprite.setScale(0.8, 0.8);
	gun_sprite.setPosition(WIDTH / 2 - gun_texs[0].getSize().x * gun_sprite.getScale().x / 2 + 20,
		HEIGHT - gun_texs[0].getSize().y * gun_sprite.getScale().x);

	gun_animation_timer = 0;
}

void Player::drawGun(float dt)
{
	
	window.draw(gun_sprite);

	gun_animation_timer += dt;

	// if frame bigger than zero, we animating
	if (gun_animation_frame && gun_animation_timer >= 0.08f)
	{
		gun_animation_frame = (gun_animation_frame + 1) % 6;
		gun_animation_timer = 0;
		gun_sprite.setTexture(gun_texs[gun_animation_frame]);
	}
}


void Player::shootGun()
{
	gun_animation_frame = 1;
	gun_sprite.setTexture(gun_texs[gun_animation_frame]);
	gun_animation_timer = 0;
}