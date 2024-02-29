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
	 shadow(size), cursor(v2f(1.5f, 30))
{
	text.setFillColor(sf::Color::Black);
	text.setPosition(position + text_offset);

	shadow.setPosition(position);
	shadow.setFillColor(sf::Color(0, 0, 0, 70));

	cursor.setFillColor(sf::Color::Black);
	cursor.setSize(v2f(1.5, 30));
}

string TextBox::getString()
{
	return text.getString();
}

void TextBox::draw(sf::RenderWindow& window, bool is_focused)
{
	string current_text = text.getString();
	string hashed(current_text.size(), '*');
	if (hidden)
	{
		text.setString(hashed);
	}
	

	window.draw(text);




	if (!is_focused)
	{
		window.draw(shadow);
		return;
	}

	if (cursor_visible)
	{
		cursor.setPosition(text.getPosition() +
			v2f(text.getGlobalBounds().getSize().x + 6, 4));
		window.draw(cursor);

		
	}


	if (cursor_timer.getElapsedTime().asMilliseconds() > 500)
	{
		cursor_visible ^= true;
		cursor_timer.restart();
	}

	if (hidden)
		text.setString(current_text);


}

void TextBox::turnOnCursor()
{
	cursor_visible = true;
	cursor_timer.restart();
}

void TextBox::addText(const string& added_text)
{
	turnOnCursor();

	text.setString(getString() + added_text);

	if (text.getString().getSize() > 16)
		text.setString(text.getString().substring(0, 16));
}

void TextBox::backspace(const int& backspace_counter)
{
	turnOnCursor();

	if (backspace_counter < 0) // ctrl backspace
	{
		text.setString("");
		return;
	}


	text.setString(text.getString()
		.substring(0, text.getString().getSize() - backspace_counter));
}

bool TextBox::inBox(const v2i& pos)
{
	return inBounds(position, size, pos);
}