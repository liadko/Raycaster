#include "headers.hpp"
#include "tools.hpp"



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

sf::Color lerp(sf::Color c1, sf::Color c2, float t)
{
	return sf::Color(
		t * c1.r + (1 - t) * c2.r,
		t * c1.g + (1 - t) * c2.g,
		t * c1.b + (1 - t) * c2.b
	);
}