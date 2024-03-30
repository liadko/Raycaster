#pragma once

#include <SFML/Network.hpp>
#include <boost/multiprecision/cpp_int.hpp>

typedef boost::multiprecision::cpp_int bigint;

using boost::multiprecision::powm;

bool tryLogIn(const string& username, const string& password, string& error);

void sendTCP(sf::TcpSocket& socket, const string& message);
bool recvTCP(sf::TcpSocket& socket, void*& payload, int& payload_size);

void sendUDP();

void PrintBytes(const char* pBytes, const uint32_t nBytes);