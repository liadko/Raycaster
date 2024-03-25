#pragma once

#include <SFML/Network.hpp>
#include <boost/multiprecision/cpp_int.hpp>

typedef boost::multiprecision::cpp_int bigint;

using boost::multiprecision::powm;

void sendUDP();
bool tryLogIn(const string& username, const string& password, string& error);

