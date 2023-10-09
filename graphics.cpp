#include "headers.hpp"
#include "graphics.hpp"


#define KEY_PRESSED(key) sf::Keyboard::isKeyPressed(sf::Keyboard::Key::key)
void handleKeys(Player& player, float dt)
{

	if (KEY_PRESSED(W))
		player.move(0, dt);
	if (KEY_PRESSED(A))
		player.move(HALF_PI, dt);
	if (KEY_PRESSED(S))
		player.move(PI, dt);
	if (KEY_PRESSED(D))
		player.move(3 * HALF_PI, dt);
	if (KEY_PRESSED(Left))
		player.rotate(-1, dt);
	if (KEY_PRESSED(Right))
		player.rotate(1, dt);

}

v2i min(v2i first, v2i second)
{
	return v2i(std::min(first.x, second.x), std::min(first.y, second.y));
}

v2i max(v2i first, v2i second)
{
	return v2i(std::max(first.x, second.x), std::max(first.y, second.y));
}

float mag(v2 vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

v2 norm(v2 vec)
{
	return vec / mag(vec);
}