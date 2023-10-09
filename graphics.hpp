#pragma once
#include "headers.hpp"
#include "player.hpp"

void handleKeys(Player& player, float dt);

v2i min(v2i first, v2i second);

v2i max(v2i first, v2i second);

float mag(v2 vec);

v2 norm(v2 vec);