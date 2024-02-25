#include "headers.hpp"
#include "tools.hpp"



v2i min(const v2i& first, const v2i& second)
{
	return v2i(std::min(first.x, second.x), std::min(first.y, second.y));
}

v2i max(const v2i& first, const v2i& second)
{
	return v2i(std::max(first.x, second.x), std::max(first.y, second.y));
}

float mag(const v2f& vec)
{
	return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

v2f norm(const v2f& vec)
{
	return vec / mag(vec);
}

float lerp(float a, float b, float t)
{
	return a * (1.0 - t) + (b * t);
}

sf::Color lerp(sf::Color c1, sf::Color c2, float t)
{
	return sf::Color(
		t * c1.r + (1 - t) * c2.r,
		t * c1.g + (1 - t) * c2.g,
		t * c1.b + (1 - t) * c2.b
	);
}

bool inBounds(const v2f& box_pos, const v2f& box_size, const v2i& pos)
{
	if (pos.x < box_pos.x || pos.x > box_pos.x + box_size.x) {
		return false;
	}

	// Check if the point's y coordinate is within the box's vertical bounds.
	if (pos.y < box_pos.y || pos.y > box_pos.y + box_size.y) {
		return false;
	}

	// If both checks pass, the point is within the box.
	return true;
}

TextBox::TextBox(const v2f& pos, const v2f& size, const string& str, const sf::Font& font)
	: position(pos), size(size), text(str, font, 30),
	in_shadow(true)
{
	text.setFillColor(sf::Color::Black);
	text.setPosition(position + text_offset);
}