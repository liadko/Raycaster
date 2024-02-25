#pragma once

#include <SFML/Network.hpp>

void sendUDP();
bool tryLogIn(const string& username, const string& password);
