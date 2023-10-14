#pragma once
#include "headers.hpp"

v2i min(v2i first, v2i second);

v2i max(v2i first, v2i second);

float mag(v2f vec);

v2f norm(v2f vec);

sf::Color lerp(sf::Color c1, sf::Color c2, float t);