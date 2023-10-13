#include "headers.hpp"
#include "graphics.hpp"


#define KEY_PRESSED(key) sf::Keyboard::isKeyPressed(sf::Keyboard::Key::key)
void handleKeys(Player& player, float dt)
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
	
	if(!(movement.x==0 && movement.y == 0))
		player.move(atan2(movement.x, movement.y), dt);

	// rotate
	if (KEY_PRESSED(Left))
		player.rotate(-1, dt);
	if (KEY_PRESSED(Right))
		player.rotate(1, dt);

	// run
	if (KEY_PRESSED(LShift))
		player.running = true;
	else
		player.running = false;
}

v2i min(v2i first, v2i second)
{
	return v2i(std::min(first.x, second.x), std::min(first.y, second.y));
}

v2i max(v2i first, v2i second)
{
	return v2i(std::max(first.x, second.x), std::max(first.y, second.y));
}

float mag(v2f vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

v2f norm(v2f vec)
{
	return vec / mag(vec);
}