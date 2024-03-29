#include "headers.hpp"
#include "player.hpp"
#include "tools.hpp"
#include "client.hpp"

Player::Player(int x, int y, Map& map, sf::RenderWindow& window) : position(x, y), map(map), window(window)
{
	loadTextures();

	gunshot_buffer.loadFromFile("sfx/9mm-pistol.wav");
	gun_sound.setBuffer(gunshot_buffer);
	gun_sound.setVolume(25);
	
	gunclick_buffer.loadFromFile("sfx/handgun-release.wav");
	click_sound.setBuffer(gunclick_buffer);

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
		movement.x += +1;
	if (KEY_PRESSED(S))
		movement.y += -1;
	if (KEY_PRESSED(D))
		movement.x += -1;

	moving = (movement.x != 0 || movement.y != 0);

	if (moving)
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
	if (dt != -1 && mag(v2f(delta_x, delta_y))*mouse_sensitivity > 120)
	{
		cout << mag(v2f(delta_x, delta_y)) << "\n";
		return;
	}



	// horizontal
	rotation_x += delta_x * mouse_sensitivity * dt;
	//sprite.setRotation(rotation_x / PI * 180);		// update player map sprite
	map.shiftSky(delta_x * mouse_sensitivity * dt); // shift sky


	// cout << rotation_x << "\n";

	// vertical
	rotation_y += delta_y * -mouse_sensitivity * dt;


	// cap y rotation
	if (rotation_y > -0.15f)
		rotation_y = -0.15f;

	if (rotation_y < -1.05f)
		rotation_y = -1.05f;

	map.floor_level = HEIGHT / 2 * (1 + tan(rotation_y)) / tan(fov_y / 2);

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
				//if (sin(rotation_x) > 0) texture_x = 1 - texture_x;
			}
			else
			{
				texture_x = hit_position.y - floor(hit_position.y);
				//if (cos(rotation_x) < 0) texture_x = 1 - texture_x;
			}


			return { distance, latest_hit_on_x, texture_x };
		}

	}


}


void Player::drawObject(Object& object)
{

	float projection_distance = 0.5f * map.cell_size / tan(fov_y/2);
	//We're getting Steven's direction relative to ours.



	float angle_to_object = atan2(position.y - object.position.y, object.position.x - position.x ) + rotation_x;

	//cout << angle_to_object << "\n";

	//float shifted_direction = object.direction + 0.5f * (PI + frame_angle) - rotation_x - steven_direction;
	float steven_projection_position = 0.5f * tan(angle_to_object) / tan(fov_x / 2);

	float screen_x = WIDTH * (0.5f - steven_projection_position);

	float steven_distance = sqrt(pow(position.x - object.position.x, 2) + pow(position.y - object.position.y, 2));
	float screen_size = HEIGHT * projection_distance / (steven_distance * cos(angle_to_object));

	screen_size *= object.size_multiplier;

	if (screen_size < 0) // behind us
		return;

	object.sprite.setScale(screen_size / map.cell_size, screen_size / map.cell_size);
	object.sprite.setPosition(
		screen_x		- object.tex_size.x * object.sprite.getScale().x / 2,
		map.floor_level - object.tex_size.y * object.sprite.getScale().y / 2);


	window.draw(object.sprite);
}


void Player::shootRays(Player::HitInfo*& hits)
{
	
	// For each column of pixels
	for (int x = 0; x < WIDTH; x++)
	{
		float y = x / float(WIDTH - 1) - 0.5f;
		float angle = atan(2* y * tan(fov_x / 2));
		HitInfo hit_info = shootRay(angle);


		hits[x] = hit_info;
		hits[x].perceived_distance = hit_info.distance * cos(angle);
		

	}

}

void Player::drawWorld(HitInfo*& hits, vector<Object>& objects)
{
	
	//Sort objects and get the distances
	bool column_drawn[WIDTH];
	for (int x = 0; x < WIDTH; x++)
		column_drawn[x] = false;

	for(int i = 0; i < objects.size(); i++)
		for (int j = 0; j < objects.size()-1; j++)
		{
			if (objects[j].distFrom(position) < objects[j + 1].distFrom(position))
			{
				Object temp = objects[j];
				objects[j] = objects[j+1];
				objects[j+1] = temp;
			}
		}


	/*cout << "Objects: ";
	for (int i = 0; i < objects.size(); i++)
	{
		cout << objects[i].distFrom(position) << " ";
	}
	cout << "enough of objects\n";*/

	for (Object& object : objects)
	{
		float object_distance = object.distFrom(position);
		for (int x = 0; x < WIDTH; x++)
		{
			if (!column_drawn[x] && hits[x].distance > object_distance)
			{
				drawColumn(x, hits[x]);
				column_drawn[x] = true;
			}

		}

		drawObject(object);
	}

	for (int x = 0; x < WIDTH; x++)
	{
		if (!column_drawn[x])
		{
			drawColumn(x, hits[x]);
		}

	}
}

void Player::drawColumn(int x, const Player::HitInfo& hit_info)
{
	float len = 1000 / hit_info.perceived_distance;

	if (hit_info.on_x_axis)
		wall_sprite.setTexture(wall_texs[1]);
	else
		wall_sprite.setTexture(wall_texs[0]);


	// drawing 
	wall_sprite.setTextureRect(sf::IntRect(
		v2i(hit_info.texture_x * wall_texs[0].getSize().x, 0), v2i(1, wall_texs[0].getSize().y)
	));
	wall_sprite.setScale(1, len / wall_texs[0].getSize().y);
	wall_sprite.setPosition(x, map.floor_level - len / 2);
	window.draw(wall_sprite);
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
	wall.loadFromFile("sprites/1L.png");
	wall_texs[0] = wall;
	wall.loadFromFile("sprites/1D.png");
	wall_texs[1] = wall;


	// gun
	gun_texs = new sf::Texture[5]();

	for (int i = 0; i < 5; i++)
	{
		char filename[] = "gun_animation/gun_X.png";
		filename[18] = i + '0';
		gun_texs[i].loadFromFile(filename);
	}

	gun_sprite.setTexture(gun_texs[0]);
	gun_sprite.setScale(0.8, 0.8);
	gun_position = { WIDTH / 2 - gun_texs[0].getSize().x * gun_sprite.getScale().x / 2 + 25,
	HEIGHT - gun_texs[0].getSize().y * gun_sprite.getScale().x + 30 };
	gun_sprite.setPosition(gun_position);

	gun_animation_timer = 0;
	gun_movement_stopwatch = 0;

	gun_animation_duration = new float[5];
	gun_animation_duration[1] = 0.08f;
	gun_animation_duration[2] = 0.12f;
	gun_animation_duration[3] = gun_animation_duration[4] = 0.2f;
}




// draws the hand holding the gun at the bottom of the screen
// dt - deltaTime, the time in seconds since the beginning of the last frame
void Player::drawGun(float dt)
{
	gun_animation_timer += dt;
	gun_movement_stopwatch += dt * 7;
	
	if (moving)
		hand_move_range = lerp(hand_move_range, max_hand_range, 0.007f);
	else
		hand_move_range = lerp(hand_move_range, 0, 0.005f);
	

	float gun_offset_x = sin(gun_movement_stopwatch) * hand_move_range;
	float gun_offset_y = 0.3f * cos(gun_movement_stopwatch) * cos(gun_movement_stopwatch) * hand_move_range;
	gun_offset = { gun_offset_x , gun_offset_y };

	gun_sprite.setPosition(gun_position + gun_offset);

	//cout << "\n";
	window.draw(gun_sprite);

	// if frame bigger than zero, we animating
	if (gun_animation_frame && gun_animation_timer >= gun_animation_duration[gun_animation_frame])
	{
		gun_animation_frame = (gun_animation_frame + 1) % 5;
		gun_animation_timer = 0;
		gun_sprite.setTexture(gun_texs[gun_animation_frame]);
	}
}


void Player::shootGun(bool left_click)
{
	if (!left_click)
	{
		click_sound.play();
		return;
	}

	sendUDP();

	
	gun_sound.play();

	gun_animation_frame = 1;
	gun_sprite.setTexture(gun_texs[gun_animation_frame]);
	gun_animation_timer = 0;
}




void Player::debug()
{
	cout << position.x << ": X\n"
		 << position.y << ": Y\n"
		 << map.floor_level << ": map floor level\n\n";
}